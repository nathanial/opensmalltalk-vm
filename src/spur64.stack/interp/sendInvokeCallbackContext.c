/* Extracted from interp.c:62798 (function sendInvokeCallbackContext). */

/*	Send the calllback message to Alien class with the supplied arg(s). Use
        either the 1 arg
        invokeCallbackContext: or the 4 arg
   invokeCallback:stack:registers:jmpbuf: message, depending on what selector is
   installed in the specialObjectsArray. Note that if invoking the legacy
   invokeCallback:stack:registers:jmpbuf: we pass the vmCallbackContext as the
   jmpbuf argument (see reestablishContextPriorToCallback:). The arguments are
   raw C addresses and are converted to integer objects on the way.
   sendInvokeCallbackContext: & returnAs:ThroughCallback:Context: along with
   ownVM: and disownVM: conspire to save and restore newMethod, argumentCount
   and primitiveFunctionPointer around a callback. The VM depends on
   argumentCount being correct to cut-back the correct number of arguments on
   primitive return. Since this is an implicit send we need to log it
   explicitly. The return side is done via a primitive so that gets logged
   normally. */

/* StackInterpreter>>#sendInvokeCallbackContext: */

sqInt sendInvokeCallbackContext(VMCallbackContext *vmCallbackContext) {
  sqInt classTag;
  usqInt i;
  sqInt methodHeader;
  usqInt numArgs;
  usqInt numTemps;
  sqInt object;
  sqInt rcvr;
  char *sp;

  /* begin recordTrace:thing:source: */
  if (TraceLog) {
    traceLog[traceLogIndex] = TraceVMCallback;
    traceLog[traceLogIndex + 1] = (longAt(
        (void *)((specialObjectsOop + BaseHeaderSize) +
                 ((((usqInt)(SelectorInvokeCallback) << (shiftForWord())))))));
    traceLog[traceLogIndex + 2] = 0;
    traceLogIndex = (traceLogIndex + 3) % TraceBufferSize;
  }

  /* begin fetchClassTagOfNonImm: */
  classTag = (longAt((void *)(longAt(
                 (void *)((specialObjectsOop + BaseHeaderSize) +
                          ((((usqInt)(ClassAlien) << (shiftForWord()))))))))) &
             (classIndexMask());
  messageSelector = longAt(
      (void *)((specialObjectsOop + BaseHeaderSize) +
               ((((usqInt)(SelectorInvokeCallback) << (shiftForWord()))))));
  if (!(lookupInMethodCacheSelclassTag(messageSelector, classTag))) {
    if (lookupOrdinaryNoMNUEtcInClass(classForClassTag(classTag))) {
      return 0;
    }
  }
  assert(((debugCallbackInvokes += 1)) > 0);

  /* self assert: debugCallbackInvokes < 3802. */

  /* begin saveCStackStateForCallbackContext: */
  memcpy(((void *)((vmCallbackContext->savedReenterInterpreter))),
         reenterInterpreter, sizeof(jmp_buf));
  longAtput((sp = stackPointer - BytesPerWord),
            longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                            ((((usqInt)(ClassAlien) << (shiftForWord())))))));
  stackPointer = sp;
  if ((argumentCountOf(newMethod)) == 4) {
    object = positive64BitIntegerFor(((usqInt)((vmCallbackContext->thunkp))));

    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord), object);
    stackPointer = sp;
    object = positive64BitIntegerFor(((usqInt)((vmCallbackContext->stackp))));

    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord), object);
    stackPointer = sp;
    object =
        positive64BitIntegerFor(((usqInt)((vmCallbackContext->intregargsp))));

    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord), object);
    stackPointer = sp;
  }
  object = positive64BitIntegerFor(((usqInt)vmCallbackContext));

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), object);
  stackPointer = sp;

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

  /* begin checkForStackOverflow */
  /* begin externalWriteBackHeadFramePointers */
  assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
  assert(stackPage == (mostRecentlyUsedPage));
  assert(!((isFree(stackPage))));

  /* begin setHeadFP:andSP:inPage: */
  assert(stackPointer < framePointer);
  assert((stackPointer < ((stackPage->baseAddress))) &&
         (stackPointer >
          (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
  assert((framePointer < ((stackPage->baseAddress))) &&
         (framePointer > (((stackPage->realStackLimit)) -
                          ((LargeContextSlots * BytesPerOop) / 2))));
  (stackPage->headFP = framePointer);
  (stackPage->headSP = stackPointer);
  assert(pageListIsWellFormed());
  if (stackPointer < ((stackPage->realStackLimit))) {
    handleStackOverflow();
  }
  assert((frameReceiver(framePointer)) == (splObj(ClassAlien)));

  /* begin enterSmalltalkExecutiveFromCallback */
  enterSmalltalkExecutive();

  /* not reached */
  return 1;
}