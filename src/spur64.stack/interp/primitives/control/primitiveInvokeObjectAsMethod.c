/* Extracted from interp.c:18469 (function primitiveInvokeObjectAsMethod). */

/*	Primitive. 'Invoke' an object like a function, sending the special
   message run: originalSelector with: arguments in: aReceiver.
         */

/* InterpreterPrimitives>>#primitiveInvokeObjectAsMethod */

static void primitiveInvokeObjectAsMethod(void) {
  sqInt i;
  usqInt iUsqInt;
  sqInt lookupClassTag;
  sqInt methodHeader;
  usqInt newObj;
  usqInt numArgs;
  usqInt numBytes;
  usqInt numTemps;
  sqInt object;
  sqInt rcvr;
  sqInt runArgs;
  sqInt runReceiver;
  char *sp;
  sqInt tagBits;
  sqInt top;
  sqInt valuePointer;

  /* begin eeInstantiateClassIndex:format:numSlots: */
  assert((argumentCount >= 0) &&
         ((knownClassAtIndex(ClassArrayCompactIndex)) != nilObj));
  assert((arrayFormat()) ==
         (instSpecOfClass(knownClassAtIndex(ClassArrayCompactIndex))));

  /* begin allocateNewSpaceSlots:format:classIndex: */
  if (argumentCount >= (numSlotsMask())) {
    if (argumentCount > 0xFFFFFFFFU) {
      runArgs = ((usqInt)null);
      goto l1;
    }
    newObj = freeStart + BaseHeaderSize;
    numBytes =
        (BaseHeaderSize + BaseHeaderSize) + (argumentCount * BytesPerOop);
  } else {
    newObj = freeStart;
    numBytes =
        BaseHeaderSize + ((argumentCount < 1 ? 8 /* allocationUnit */
                                             : argumentCount * BytesPerOop));
  }
  if ((freeStart + numBytes) > scavengeThreshold) {
    if (!needGCFlag) {
      /* begin scheduleScavenge */
      needGCFlag = 1;
      forceInterruptCheck();
    }
    if ((freeStart + numBytes) > (((eden).limit))) {
      error("no room in eden for allocateNewSpaceSlots:format:classIndex:");
      runArgs = 0;
      goto l1;
    }
  }
  if (argumentCount >= (numSlotsMask())) {
    longAtput((void *)(freeStart), argumentCount);
    longAtput((void *)(freeStart + 4),
              ((sqInt)((usqInt)((numSlotsMask())) << (numSlotsHalfShift()))));
    long64Atput((void *)(newObj),
                ((((((usqLong)(numSlotsMask()))) << (numSlotsFullShift()))) +
                 ((((usqInt)((arrayFormat())) << (formatShift()))))) +
                    ClassArrayCompactIndex);
  } else {
    long64Atput((void *)(newObj),
                ((((((usqLong)argumentCount)) << (numSlotsFullShift()))) +
                 ((((usqInt)((arrayFormat())) << (formatShift()))))) +
                    ClassArrayCompactIndex);
  }

  /* for header parsing we put a saturated slot count in the prepended overflow
   * size word */
  assert((numBytes % (allocationUnit())) == 0);
  assert((newObj % (allocationUnit())) == 0);
  freeStart += numBytes;
  runArgs = newObj;
  /* end eeInstantiateClassIndex:format:numSlots: */
l1:
  for (i = (argumentCount - 1); i >= 0; i += -1) {
    /* begin popStack */
    top = longAt(stackPointer);
    stackPointer += BytesPerWord;
    valuePointer = top;

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(runArgs)) && (!(isForwarded(runArgs))));
    assert(validStorePointerUncheckedArgs(i, runArgs, valuePointer));
    longAtput((void *)((runArgs + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              valuePointer);
  }

  /* begin popStack */
  runReceiver = longAt(stackPointer);
  stackPointer += BytesPerWord;

  /* setup send of newMethod run: originalSelector with: runArgs in: runReceiver
   */

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), newMethod);
  stackPointer = sp;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), messageSelector);
  stackPointer = sp;

  /* original selector */

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), runArgs);
  stackPointer = sp;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), runReceiver);
  stackPointer = sp;

  /* stack is clean here */
  messageSelector = fetchPointerofObject(SelectorRunWithIn, specialObjectsOop);
  argumentCount = 3;
  lookupClassTag = /* fetchClassTagOf: */
      ((tagBits = newMethod & (tagMask()))
           ? tagBits
           : (longAt((void *)(newMethod))) & (classIndexMask()));
  findNewMethodInClassTag(lookupClassTag);

  /* begin executeNewMethod */
  if (primitiveFunctionPointer) {
    if ((((usqIntptr_t)primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex) {
      externalQuickPrimitiveResponse();
      goto l2;
    }
    if (slowPrimitiveResponse()) {
      goto l2;
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
l2:

  /* begin initPrimCall */
  primFailCode = 0;
}