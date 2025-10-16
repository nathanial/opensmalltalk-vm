/* Extracted from interp.c:63601 (function snapshot). */

/*	update state of active context */

/* StackInterpreter>>#snapshot: */

static sqInt snapshot(sqInt embedded) {
  sqInt activeContext;
  sqInt activeProc;
  usqInt i;
  sqInt methodHeader;
  usqInt numArgs;
  usqInt numTemps;
  sqInt object;
  sqInt objOop;
  sqInt oop;
  sqInt rcvr;
  sqInt rcvrSqInt;
  sqInt savedTenuringThreshold;
  void *setMacType;
  char *sp;
  sqInt stackIndex;

  /* For now the stack munging below doesn't deal with more than one argument.
     It can, and should. */
  rcvr = 0;
  if (argumentCount) {
    return (primFailCode = PrimErrBadNumArgs);
  }

  /* Need to convert all frames into contexts since the snapshot file only holds
   * objects. */

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), instructionPointer);
  stackPointer = sp;
  activeContext = voidVMStateForSnapshotFlushingExternalPrimitivesIf(1);

  /* update state of active process */

  /* begin activeProcess */
  objOop = fetchPointerofObject(
      ValueIndex,
      fetchPointerofObject(SchedulerAssociation, specialObjectsOop));
  activeProc = fetchPointerofObject(ActiveProcessIndex, objOop);

  storePointerofObjectwithValue(SuspendedContextIndex, activeProc,
                                activeContext);
  tempOop = activeContext;

  /* begin garbageCollectForSnapshot */
  /* begin flushNewSpace */
  savedTenuringThreshold = tenureThreshold;
  tenureThreshold = oldSpaceStart;
  scavengingGCTenuringIf(TenureByAge);

  /* begin setRawTenuringThreshold: */
  tenureThreshold = savedTenuringThreshold;
  assert((rememberedSetSize) == 0);
  assert(pastSpaceStart == (((pastSpace).start)));
  assert(freeStart == (((eden).start)));

  /* begin biasForSnapshot */
  biasForGC = 0;
  fullGC();

  /* begin biasForGC */
  biasForGC = 1;
  unfireQueuedEphemeronsForSnapshot();
  prepareForSnapshot();

  /* begin checkFreeSpace: */
  assert(bitsSetInFreeSpaceMaskForAllFreeLists());
  assert(totalFreeOldSpace == (totalFreeListBytes()));
  if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) ==
       (GCCheckFreeSpace | GCModeFull))) {
    runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
  }

  /* Nothing moves from here on so it is safe to grab the activeContext again.
   */
  activeContext = tempOop;
  tempOop = 0;
  if (!primFailCode) {
    /* begin quickFetchInteger:ofObject: */
    oop = fetchPointerofObject(StackPointerIndex, activeContext);
    assert((((oop) & 7) == 1));
    stackIndex = (oop >> 3);
    rcvr = fetchPointerofObject((stackIndex + CtxtTempFrameStart) - 1,
                                activeContext);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(activeContext)) && (!(isForwarded(activeContext))));
    assert(validStorePointerUncheckedArgs((stackIndex + CtxtTempFrameStart) - 1,
                                          activeContext, trueObj));
    longAtput((void *)((activeContext + BaseHeaderSize) +
                       ((((usqInt)(((stackIndex + CtxtTempFrameStart) - 1))
                          << (shiftForWord()))))),
              trueObj);

    /* now attempt to write the snapshot file */
    writeImageFileIO();
    if ((!primFailCode) && (!embedded)) {
      setMacType = ioLoadFunctionFrom("setMacFileTypeAndCreator", "FilePlugin");
      if (setMacType) {
        ((sqInt (*)(char *, char *, char *))setMacType)(imageName, "STim",
                                                        "FAST");
      }
    }

    /* set Mac file type and creator; this is a noop on other platforms
       Without contexts or stacks simulate
       self pop: 1 */

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(activeContext)) && (!(isForwarded(activeContext))));
    assert(
        validStorePointerUncheckedArgs(StackPointerIndex, activeContext,
                                       (((usqInt)(stackIndex - 1) << 3) | 1)));
    longAtput((void *)((activeContext + BaseHeaderSize) +
                       ((((usqInt)(StackPointerIndex) << (shiftForWord()))))),
              (((usqInt)(stackIndex - 1) << 3) | 1));
  }

  /* Without contexts or stacks simulate
     rcvr := self popStack. ''pop rcvr''
     self push: trueObj.
     to arrange that the snapshot resumes with true.  N.B. stackIndex is
     one-relative. */

  /* begin postSnapshot */
  restorePostSnapshot();
  refireQueuedEphemeronsPostSnapshot();
  marryContextInNewStackPageAndInitializeInterpreterRegisters(activeContext);
  if (primFailCode) {
    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord), rcvr);
    stackPointer = sp;

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

    /* Skip the CallPrimitive bytecode, if it's there, and store the error code
       if the method starts with a long store temp.  Strictly no need to skip
       the store because it's effectively a noop. */
  } else {
    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord), falseObj);
    stackPointer = sp;
  }
  return 0;
}