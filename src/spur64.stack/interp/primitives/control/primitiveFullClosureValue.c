/* Extracted from interp.c:16652 (function primitiveFullClosureValue). */

/* InterpreterPrimitives>>#primitiveFullClosureValue */

static void primitiveFullClosureValue(void) {
  sqInt blockClosure;
  sqInt closureMethod;
  sqInt i;
  sqInt methodHeader;
  sqInt methodHeaderSqInt;
  sqInt numArgs;
  sqInt numCopied;
  usqInt numSlots;
  usqInt numTemps;
  sqInt object;
  sqInt objOop;
  sqInt oop;
  char *sp;

  blockClosure = longAt(stackPointer + (argumentCount * BytesPerWord));

  /* begin argumentCountOfClosure: */
  /* begin quickFetchInteger:ofObject: */
  oop = fetchPointerofObject(ClosureNumArgsIndex, blockClosure);
  assert((((oop) & 7) == 1));
  numArgs = (oop >> 3);
  if (!(argumentCount == numArgs)) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  closureMethod =
      fetchPointerofObject(FullClosureCompiledBlockIndex, blockClosure);
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

  /* begin activateNewFullClosure:method:numArgs:mayContextSwitch: */
  assert(closureMethod ==
         (fetchPointerofObject(FullClosureCompiledBlockIndex, blockClosure)));
  assert(!((isVanillaBlockClosure(blockClosure))));
  numCopied =
      ((
        assert((classIndexOf(blockClosure)) >
               (isForwardedObjectClassIndexPun())),
        numSlotsOf(blockClosure))) -
      FullClosureFirstCopiedValueIndex;

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

  objOop = followFieldofObject(FullClosureReceiverIndex, blockClosure);
  object = objOop;

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), object);
  stackPointer = sp;

  /* Copy the copied values... */
  for (i = 0; i < numCopied; i += 1) {
    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord),
              fetchPointerofObject(i + FullClosureFirstCopiedValueIndex,
                                   blockClosure));
    stackPointer = sp;
  }
  assert(frameIsBlockActivation(framePointer));
  assert(!(frameHasContext(framePointer)));

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(closureMethod));
  methodHeader = fetchPointerofObject(HeaderIndex, closureMethod);
  numTemps = (((usqInt)(methodHeader)) >> MethodHeaderTempCountShift) & 0x3F;
  for (i = ((numArgs + numCopied) + 1); i <= numTemps; i += 1) {
    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord), nilObj);
    stackPointer = sp;
  }
  instructionPointer =
      (((((usqInt)(pointerForOop(closureMethod)))) +
        ((LiteralStart +
          ((/* begin literalCountOfMethodHeader: */
            assert((((methodHeader) & 7) == 1)),
            /* literalCountOfAlternateHeader: */
            ((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask))) *
         BytesPerOop)) +
       BaseHeaderSize) -
      1;
  method = closureMethod;
  assert(isOopCompiledMethod(method));

  /* begin methodUsesAlternateBytecodeSet: */
  /* begin methodHeaderOf: */
  assert(isCompiledMethod(method));
  methodHeaderSqInt = fetchPointerofObject(HeaderIndex, method);
  if ((((sqLong)methodHeaderSqInt)) < 0) {
    bytecodeSetSelector = 0x100;
  } else {
    bytecodeSetSelector = 0;
  }

  /* Now check for stack overflow or an event (interrupt, must scavenge, etc) */
  if (stackPointer < stackLimit) {
    handleStackOverflowOrEventAllowContextSwitch(1);
  }
}