/* Extracted from interp.c:67994 (function primitiveExecuteMethodArgsArray). */

/*	receiver, argsArray, then method are on top of stack. Execute method
        against receiver and args. Allow for up to two extra arguments (e.g. for
        mirror primitives).
        Set primitiveFunctionPointer because no cache lookup has been done for
   the method, and hence primitiveFunctionPointer is stale. */

/* StackInterpreterPrimitives>>#primitiveExecuteMethodArgsArray */

static void primitiveExecuteMethodArgsArray(void) {
  usqInt argCnt;
  sqInt argumentArray;
  usqInt firstBytecode;
  sqInt header;
  sqInt i;
  usqInt iUsqInt;
  sqInt methodArgument;
  sqInt methodHeader;
  usqInt numArgs;
  usqInt numTemps;
  sqInt object;
  sqInt primitiveIndex;
  sqInt rcvr;
  sqInt rcvrSqInt;
  char *sp;

  methodArgument = longAt(stackPointer);
  argumentArray = longAt(stackPointer + (1 * BytesPerWord));
  if (!((isOopCompiledMethod(methodArgument)) &&
        (/* isArray: */
         ((!(argumentArray & (tagMask())))) &&
         (((byteAt((void *)(argumentArray + (formatFieldByteOffset())))) &
           (formatMask())) == (arrayFormat()))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }

  /* begin argumentCountOf: */
  header = methodHeaderOf(methodArgument);
  argCnt = (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;
  if (!(argCnt == ((assert((classIndexOf(argumentArray)) >
                           (isForwardedObjectClassIndexPun())),
                    numSlotsOf(argumentArray))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    return;
  }
  if (argumentCount > 2) {
    rcvr = longAt(stackPointer + (2 * BytesPerWord));
    if ((argumentCount > 4) || (isOopForwarded(rcvr))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrUnsupported;
      return;
    }

    /* stackValue:put: */
    longAtput(stackPointer + (argumentCount * BytesPerWord), rcvr);
  }

  /* begin pop: */
  stackPointer += argumentCount * BytesPerWord;
  for (i = 0; i < argCnt; i += 1) {
    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord),
              fetchPointerofObject(i, argumentArray));
    stackPointer = sp;
  }
  newMethod = methodArgument;

  /* begin primitiveIndexOf: */
  methodHeader = methodHeaderOf(newMethod);
  if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
    firstBytecode =
        (newMethod + ((LiteralStart + (((methodHeader >> 3)) &
                                       AlternateHeaderNumLiteralsMask)) *
                      BytesPerOop)) +
        BaseHeaderSize;
    primitiveIndex = (byteAt((void *)(firstBytecode + 1))) +
                     ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
  } else {
    primitiveIndex = 0;
  }

  /* begin functionPointerFor:inClass: */
  primitiveFunctionPointer =
      ((void (*)(void))(((((usqInt)primitiveIndex)) > MaxPrimitiveIndex
                             ? 0
                             : primitiveTable[primitiveIndex])));
  argumentCount = argCnt;

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
  methodHeader = methodHeaderOf(newMethod);
  numTemps = (((usqInt)(methodHeader)) >> MethodHeaderTempCountShift) & 0x3F;
  numArgs = (((usqInt)(methodHeader)) >> MethodHeaderArgCountShift) & 15;

  /* could new rcvr be set at point of send? */
  rcvrSqInt = longAt(stackPointer + (numArgs * BytesPerWord));
  assert(!(isOopForwarded(rcvrSqInt)));

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
  longAtput((sp = stackPointer - BytesPerWord), rcvrSqInt);
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