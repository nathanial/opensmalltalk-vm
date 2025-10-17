/* Extracted from interp.c:13645 (function primitiveClosureValue). */

/* InterpreterPrimitives>>#primitiveClosureValue */

static void primitiveClosureValue(void) {
  sqInt blockClosure;
  sqInt closureIP;
  sqInt closureMethod;
  sqInt i;
  sqInt methodHeader;
  sqInt numArgs;
  sqInt numCopied;
  usqInt numSlots;
  sqInt object;
  sqInt objOop;
  sqInt oop;
  sqInt outerContext;
  char *sp;

  blockClosure = longAt(stackPointer + (argumentCount * BytesPerWord));

  /* begin argumentCountOfClosure: */
  /* begin quickFetchInteger:ofObject: */
  oop =
      fetchPointerofObject(ClosureNumArgsIndex, blockClosure);
  assert((((oop) & 7) == 1));
  numArgs = (oop >> 3);
  if (!(argumentCount == numArgs)) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

  /* Somewhat paranoiac checks we need while debugging that we may be able to
     discard in a robust system. */
  outerContext = fetchPointerofObject(ClosureOuterContextIndex, blockClosure);
  if (!(/* isContext: */
        ((!(outerContext & (tagMask())))) &&
        (((longAt((void *)(outerContext))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  closureMethod =
      fetchPointerofObject(MethodIndex, outerContext);

  /* Check if the closure's method is actually a CompiledMethod. */
  if (!(/* isOopCompiledMethod: */
        ((!(closureMethod & (tagMask())))) &&
        (((byteAt((void *)(closureMethod + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstCompiledMethodFormat())))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

  /* begin activateNewClosure:outer:method:numArgs:mayContextSwitch: */
  assert(isContext(outerContext));
  assert(isVanillaBlockClosure(blockClosure));
  numCopied =
      ((/* begin numSlotsOf: */
        assert((classIndexOf(blockClosure)) >
               (isForwardedObjectClassIndexPun())),
        (((numSlots =
               byteAt((void *)(blockClosure + (numSlotsFieldByteOffset()))))) ==
                 (numSlotsMask())
             ? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(blockClosure -
                                                              BaseHeaderSize))))
                                    << 8)))))) >>
                   8
             : numSlots))) -
      ClosureFirstCopiedValueIndex;
  assert(closureMethod == (fetchPointerofObject(MethodIndex, outerContext)));
  assert(isOopCompiledMethod(closureMethod));

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), instructionPointer);
  stackPointer = sp;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), ((usqInt)framePointer));
  stackPointer = sp;
  framePointer = stackPointer;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), closureMethod);
  stackPointer = sp;
  object = /* encodeFrameFieldHasContext:isBlock:numArgs: */
      (VMBIGENDIAN
           ? ((1 + ((((usqInt)(numArgs) << ((BytesPerWord * 8) - 8)))))) +
                 (1ULL << ((BytesPerWord * 8) - 24))
           : ((1 + ((((usqInt)(numArgs) << 8))))) + (0x1000000));

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), object);
  stackPointer = sp;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), nilObj);
  stackPointer = sp;

  /* begin followField:ofObject: */
  objOop = fetchPointerofObject(ReceiverIndex, outerContext);
  if (isOopForwarded(objOop)) {
    objOop = fixFollowedFieldofObjectwithInitialValue(ReceiverIndex,
                                                      outerContext, objOop);
  }
  object = objOop;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), object);
  stackPointer = sp;

  /* Copy the copied values... */
  for (i = 0; i < numCopied; i += 1) {
    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord),
              fetchPointerofObject(i + ClosureFirstCopiedValueIndex, blockClosure));
    stackPointer = sp;
  }
  assert(frameIsBlockActivation(framePointer));
  assert(!(frameHasContext(framePointer)));

  /* The initial instructions in the block nil-out remaining temps.
     the instruction pointer is a pointer variable equal to
     method oop + ip + BaseHeaderSize
     -1 for 0-based addressing of fetchByte
     -1 because it gets incremented BEFORE fetching currentByte */

  /* begin quickFetchInteger:ofObject: */
  oop =
      fetchPointerofObject(ClosureStartPCIndex, blockClosure);
  assert((((oop) & 7) == 1));
  closureIP = (oop >> 3);
  instructionPointer = ((closureMethod + closureIP) + BaseHeaderSize) - 2;

  /* begin setMethod: */
  method = closureMethod;
  assert(isOopCompiledMethod(method));

  /* begin methodUsesAlternateBytecodeSet: */
  /* begin methodHeaderOf: */
  assert(isCompiledMethod(method));
  methodHeader =
      fetchPointerofObject(HeaderIndex, method);
  if ((((sqLong)methodHeader)) < 0) {
    bytecodeSetSelector = 0x100;
  } else {
    bytecodeSetSelector = 0;
  }

  /* Now check for stack overflow or an event (interrupt, must scavenge, etc) */
  if (stackPointer < stackLimit) {
    handleStackOverflowOrEventAllowContextSwitch(1);
  }
}