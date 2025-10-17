/* Extracted from interp.c:52456 (function executeNewMethod). */

/*	Execute newMethod - either primitiveFunctionPointer must be set directly
        (i.e. from primitiveExecuteMethod et al), or it would have been set
        probing the method cache (i.e. primitivePerform et al). */

/* StackInterpreter>>#executeNewMethod */

static sqInt executeNewMethod(void) {
  usqInt i;
  sqInt methodHeader;
  usqInt numArgs;
  usqInt numTemps;
  sqInt object;
  sqInt rcvr;
  char *sp;

  if (primitiveFunctionPointer) {
    if ((((usqIntptr_t)primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex) {
      externalQuickPrimitiveResponse();
      return null;
    }
    if (slowPrimitiveResponse()) {
      return null;
    }
  }

  /* if not primitive, or primitive failed, activate the method */

  /* begin activateNewMethod */
  /* begin justActivateNewMethod: */
  /* begin methodHeaderOf: */
  assert(isCompiledMethod(newMethod));
  methodHeader =
      longAt((void *)((newMethod + BaseHeaderSize) +
                      ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
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
  return 0;
}