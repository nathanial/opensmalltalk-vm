/* Extracted from interp.c:20436 (function primitivePerformInSuperclass). */

/* InterpreterPrimitives>>#primitivePerformInSuperclass */

static void primitivePerformInSuperclass(void) {
  sqInt arg;
  sqInt argumentArray;
  usqInt arraySize;
  sqInt currentClass;
  sqInt delta;
  sqInt index;
  sqInt lookupClass;
  usqInt numSlots;
  sqInt objOop;
  sqInt offset;
  sqInt performArgCount;
  sqInt rcvr;
  usqInt savedNewMethod;
  sqInt selector;
  char *sp;
  sqInt tagBits;
  sqInt top;

  rcvr = longAt(stackPointer + (3 * BytesPerWord));
  lookupClass = longAt(stackPointer);
  if (argumentCount != 3) {
    if (argumentCount != 4) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadNumArgs;
      return;
    }
    if (isOopForwarded(rcvr)) {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
      return;
    }
  }

  /* e.g. object:perform:withArguments:inClass: */
  currentClass = /* fetchClassOf: */
      ((tagBits = rcvr & (tagMask()))
           ? fetchPointerofObject(tagBits, classTableFirstPage)
           : fetchClassOfNonImm(rcvr));
  while (currentClass != lookupClass) {
    /* begin superclassOf: */
    /* begin followObjField:ofObject: */
    objOop =
        fetchPointerofObject(SuperclassIndex, currentClass);
    assert(isNonImmediate(objOop));
    if ((!((longAt((void *)(objOop))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex,
                                                        currentClass, objOop);
    }
    currentClass = objOop;
    if (currentClass == nilObj) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      return;
    }
  }
  selector = longAt(stackPointer + (2 * BytesPerWord));
  argumentArray = longAt(stackPointer + (1 * BytesPerWord));

  /* begin primitiveObject:perform:withArguments:lookedUpIn: */
  if (!(/* isArray: */
        ((!(argumentArray & (tagMask())))) &&
        (((byteAt((void *)(argumentArray + (formatFieldByteOffset())))) &
          (formatMask())) == (arrayFormat())))) {
    performWithArgumentsRecursionGuard = null;

    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    goto l1;
  }

  /* Check if number of arguments is reasonable; MaxNumArgs isn't available
     so just use LargeContextSize */

  /* begin numSlotsOf: */
  assert((classIndexOf(argumentArray)) > (isForwardedObjectClassIndexPun()));
  arraySize =
      (((numSlots =
             byteAt((void *)(argumentArray + (numSlotsFieldByteOffset()))))) ==
               (numSlotsMask())
           ? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(argumentArray -
                                                            BaseHeaderSize))))
                                  << 8)))))) >>
                 8
           : numSlots);
  if (arraySize > (LargeContextSlots - CtxtTempFrameStart)) {
    performWithArgumentsRecursionGuard = null;

    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    goto l1;
  }
  performArgCount = argumentCount;

  /* Recursion check:
     | a |
     a := Array with: #perform:withArguments: with: nil.
     a at: 2 put: a.
     a perform: a first withArguments: a.
     | a |
     a := Array with: #perform:withArguments: with: (Array with:
     #perform:withArguments: with: nil). a last at: 2 put: a. a perform: a first
     withArguments: a */
  if (!performWithArgumentsRecursionGuard) {
    performWithArgumentsRecursionGuard =
        (lookupClass ? longAt(stackPointer + (1 * BytesPerWord))
                     : longAt(stackPointer));
  }

  /* Push newMethod to save it in case of failure,
     then push the actual receiver and the args in the array. */
  savedNewMethod = newMethod;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), newMethod);
  stackPointer = sp;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), rcvr);
  stackPointer = sp;

  /* Copy the arguments to the stack, in case of MNU, and lookup */
  for (index = 1; index <= arraySize; index += 1) {
    arg = fetchPointerofObject(index - 1, argumentArray);
    if (arg == performWithArgumentsRecursionGuard) {
      performWithArgumentsRecursionGuard = null;
    }

    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord), arg);
    stackPointer = sp;
  }
  if (!performWithArgumentsRecursionGuard) {
    /* begin pop: */
    stackPointer += (2 + arraySize) * BytesPerWord;

    /* primitiveFailFor: */
    primFailCode = PrimErrInappropriate;
    goto l1;
  }
  argumentCount = arraySize;
  messageSelector = selector;

  /* begin sendBreakpoint:receiver: */
  sendBreakpointclassTag(
      firstFixedFieldOfMaybeImmediate(messageSelector),
      lengthOfMaybeImmediate(messageSelector), /* fetchClassTagOf: */
      ((tagBits = rcvr & (tagMask()))
           ? tagBits
           : (longAt((void *)(rcvr))) & (classIndexMask())));
  if (
#if SEND_PRINTING
      printSends
#else
      0
#endif
  ) {
    printActivationNameForSelectorstartClass(
        messageSelector,
        (lookupClass ? lookupClass : /* fetchClassOf: */
             ((tagBits = rcvr & (tagMask()))
                  ? fetchPointerofObject(tagBits, classTableFirstPage)
                  : fetchClassOfNonImm(rcvr))));
    cr();
  }
  findNewMethodInClassTag(
      (lookupClass ? classTagForClass(lookupClass) : /* fetchClassTagOf: */
           ((tagBits = rcvr & (tagMask()))
                ? tagBits
                : (longAt((void *)(rcvr))) & (classIndexMask()))));

  /* Only test CompiledMethods for argument count - any other objects playacting
   * as CMs will have to take their chances */
  if ((/* isOopCompiledMethod: */
       ((!(newMethod & (tagMask())))) &&
       (((byteAt((void *)(newMethod + (formatFieldByteOffset())))) &
         (formatMask())) >= (firstCompiledMethodFormat()))) &&
      ((argumentCountOf(newMethod)) != argumentCount)) {
    assert((stackTop()) ==
           ((arraySize == 0
                 ? rcvr
                 : fetchPointerofObject(arraySize - 1, argumentArray))));
    assert(argumentCount == arraySize);

    /* begin pop: */
    stackPointer += (arraySize + 1) * BytesPerWord;

    /* begin popStack */
    top = longAt(stackPointer);
    stackPointer += BytesPerWord;
    newMethod = top;

    /* Must reset primitiveFunctionPointer for
     * checkForAndFollowForwardedPrimitiveState */
    argumentCount = performArgCount;
    primitiveFunctionPointer =
        (lookupClass ? primitivePerformInSuperclass : primitivePerformWithArgs);
    performWithArgumentsRecursionGuard = null;

    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    goto l1;
  }

  /* Restore the state by popping the array entries, the actual receiver and the
     saved newMethod, leaving the selector and array, and fail.  N.B.  If an MNU
     has happened then argumentCount will match newMethod, so this code will not
     be reached. These asserts check that an MNU has not occurred if the
     argumentCount doesn't match the newMethod. Cannot fail this primitive from
     here-on.  Slide the actual receiver and arguments down to replace the
     perform arguments and saved newMethod and then execute the new method. Use
     argumentCount not arraySize because an MNU may have changed it. */

  /* +2 = receiver + saved newMethod */
  delta = BytesPerWord * (performArgCount + 2);
  for (offset = (argumentCount * BytesPerWord); offset >= 0;
       offset += (-BytesPerWord)) {
    longAtput((stackPointer + offset) + delta, longAt(stackPointer + offset));
  }

  /* Part of the recursion guard above. To ensure we fail in the original
     method, restore newMethod, and don't do the activateNewMethod implicit in
     executeNewMethod. */
  if ((primitiveFunctionPointer == primitivePerformWithArgs) ||
      (primitiveFunctionPointer == primitivePerformInSuperclass)) {
    newMethod = savedNewMethod;
    argumentCount = performArgCount;

    /* begin pop: */
    stackPointer += (performArgCount + 2) * BytesPerWord;

    /* prevent inlining... */
    slowPrimitiveResponse();
    performWithArgumentsRecursionGuard = null;
    goto l1;
  }

  /* This should of course be a tail call, which could be done via
     setjmp/longjmp. But this is vanity code. After all how often is a recursive
     invocation of primitivePerformWithArgs et al made? */
  performWithArgumentsRecursionGuard = null;

  /* begin pop: */
  stackPointer += (performArgCount + 2) * BytesPerWord;

  /* prevent inlining... */
  executeNewMethod();

  /* begin initPrimCall */
  primFailCode = 0;
  /* end primitiveObject:perform:withArguments:lookedUpIn: */
l1:;
}