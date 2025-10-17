/* Extracted from interp.c:67839 (function primitiveExecuteMethod). */

/*	receiver, args, then method are on top of stack. Execute method against
        receiver and args.
        Set primitiveFunctionPointer because no cache lookup has been done for
   the method, and hence primitiveFunctionPointer is stale. */

/* StackInterpreterPrimitives>>#primitiveExecuteMethod */

static void primitiveExecuteMethod(void) {
  usqInt firstBytecode;
  usqInt i;
  sqInt methodArgument;
  sqInt methodHeader;
  usqInt numArgs;
  usqInt numTemps;
  sqInt object;
  sqInt primitiveIndex;
  sqInt rcvr;
  char *sp;
  sqInt top;

  methodArgument = longAt(stackPointer);
  if (!(/* isOopCompiledMethod: */
        ((!(methodArgument & (tagMask())))) &&
        (((byteAt((void *)(methodArgument + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstCompiledMethodFormat())))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  if (!((argumentCount - 1) == (argumentCountOf(methodArgument)))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    return;
  }

  /* begin popStack */
  top = longAt(stackPointer);
  stackPointer += BytesPerWord;
  newMethod = top;

  /* begin primitiveIndexOf: */
  /* begin methodHeaderOf: */
  assert(isCompiledMethod(newMethod));
  methodHeader =
      fetchPointerofObject(HeaderIndex, newMethod);
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
  argumentCount -= 1;

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
  methodHeader =
      fetchPointerofObject(HeaderIndex, newMethod);
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
  for (i = (numArgs + 1); i <= numTemps; i += 1) {
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