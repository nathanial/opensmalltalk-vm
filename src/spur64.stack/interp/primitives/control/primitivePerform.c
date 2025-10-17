/* Extracted from interp.c:69216 (function primitivePerform). */

/* StackInterpreterPrimitives>>#primitivePerform */

static void primitivePerform(void) {
  sqInt i;
  usqInt iUsqInt;
  sqInt lookupClassTag;
  sqInt methodHeader;
  sqInt newReceiver;
  usqInt numArgs;
  usqInt numTemps;
  sqInt object;
  usqInt performMethod;
  sqInt rcvr;
  char *sp;
  sqInt tagBits;

  performMethod = newMethod;
  messageSelector = longAt(stackPointer + ((argumentCount - 1) * BytesPerWord));
  newReceiver = longAt(stackPointer + (argumentCount * BytesPerWord));

  /* NOTE: the following lookup may fail and be converted to
     #doesNotUnderstand:, so we must adjust argumentCount and slide args now, so
     that will work. Slide arguments down over selector */
  argumentCount -= 1;
  for (i = argumentCount; i >= 1; i += -1) {
    longAtput(stackPointer + (i * BytesPerWord),
              longAt(stackPointer + ((i - 1) * BytesPerWord)));
  }

  /* begin pop: */
  stackPointer += 1 * BytesPerWord;
  lookupClassTag = /* fetchClassTagOf: */
      ((tagBits = newReceiver & (tagMask()))
           ? tagBits
           : (longAt((void *)(newReceiver))) & (classIndexMask()));

  /* begin sendBreakpoint:receiver: */
  sendBreakpointclassTag(
      firstFixedFieldOfMaybeImmediate(messageSelector),
      lengthOfMaybeImmediate(messageSelector), /* fetchClassTagOf: */
      ((tagBits = newReceiver & (tagMask()))
           ? tagBits
           : (longAt((void *)(newReceiver))) & (classIndexMask())));
  if (
#if SEND_PRINTING
      printSends
#else
      0
#endif
  ) {
    printActivationNameForSelectorstartClass(messageSelector,
                                             classForClassTag(lookupClassTag));
    cr();
  }
  findNewMethodInClassTag(lookupClassTag);

  /* Only test CompiledMethods for argument count - other objects will have to
   * take their chances */
  if (!((/* isOopCompiledMethod: */
         ((!(newMethod & (tagMask())))) &&
         (((byteAt((void *)(newMethod + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstCompiledMethodFormat()))) &&
        ((argumentCountOf(newMethod)) == argumentCount))) {
    /* begin unPop: */
    stackPointer -= 1 * BytesPerWord;
    for (i = 1; i <= argumentCount; i += 1) {
      longAtput(stackPointer + ((i - 1) * BytesPerWord),
                longAt(stackPointer + (i * BytesPerWord)));
    }
    longAtput(stackPointer + (argumentCount * BytesPerWord), messageSelector);
    argumentCount += 1;
    newMethod = performMethod;

    /* Must reset primitiveFunctionPointer for
     * checkForAndFollowForwardedPrimitiveState */
    primitiveFunctionPointer = primitivePerform;

    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    return;
  }

  /* Slide the args back up (sigh) and re-insert the selector. */

  /* begin executeNewMethod */
  if (primitiveFunctionPointer) {
    if ((((usqIntptr_t)primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex) {
      externalQuickPrimitiveResponse();
      goto l1;
    }
    if (slowPrimitiveResponse()) {
      goto l1;
    }
  }

  /* if not primitive, or primitive failed, activate the method */

  /* begin activateNewMethod */
  /* begin justActivateNewMethod: */
  /* begin methodHeaderOf: */
  assert(isCompiledMethod(newMethod));
  methodHeader = fetchPointerofObject(HeaderIndex, newMethod);
  numTemps = (((usqInt)(methodHeader)) >> MethodHeaderTempCountShift) & 0x3F;
  numArgs = (((usqInt)(methodHeader)) >> MethodHeaderArgCountShift) & 15;

  /* could new rcvr be set at point of send? */
  rcvr = longAt(stackPointer + (numArgs * BytesPerWord));
  assert(!(isOopForwarded(rcvr)));

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), instructionPointer);
  stackPointer = sp;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), ((usqInt)framePointer));
  stackPointer = sp;
  framePointer = stackPointer;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), newMethod);
  stackPointer = sp;

  /* begin setMethod:methodHeader: */
  method = newMethod;
  assert(isOopCompiledMethod(method));
  assert((methodHeaderOf(method)) == methodHeader);
  bytecodeSetSelector = ((((sqLong)methodHeader)) < 0 ? 0x100 : 0);
  object = /* encodeFrameFieldHasContext:isBlock:numArgs: */
      (VMBIGENDIAN ? ((1 + ((numArgs << ((BytesPerWord * 8) - 8)))))
                   : ((1 + ((numArgs << 8)))));

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), object);
  stackPointer = sp;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), nilObj);
  stackPointer = sp;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), rcvr);
  stackPointer = sp;

  /* clear remaining temps to nil */
  for (iUsqInt = (numArgs + 1); iUsqInt <= numTemps; iUsqInt += 1) {
    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord), nilObj);
    stackPointer = sp;
  }
  instructionPointer =
      (((((usqInt)(pointerForOop(newMethod)))) +
        ((LiteralStart +
          ((/* begin literalCountOfMethodHeader: */
            assert((((methodHeader) & 7) == 1)),
            /* literalCountOfAlternateHeader: */
            ((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask))) *
         BytesPerOop)) +
       BaseHeaderSize) -
      1;
  if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
    instructionPointer += 3 /* sizeOfCallPrimitiveBytecode: */;
    if (primFailCode) {
      reapAndResetErrorCodeToheader(stackPointer, methodHeader);
    }
  }

  /* Skip the CallPrimitive bytecode, if it's there, and store the error code if
     the method starts with a long store temp.  Strictly no need to skip the
     store because it's effectively a noop. */

  /* Now check for stack overflow or an event (interrupt, must scavenge, etc).
   */
  if (stackPointer < stackLimit) {
    handleStackOverflowOrEventAllowContextSwitch(
        canContextSwitchIfActivatingheader(newMethod, methodHeader));
  }
  /* end executeNewMethod */
l1:

  /* Recursive xeq affects primErrorCode */

  /* begin initPrimCall */
  primFailCode = 0;
}