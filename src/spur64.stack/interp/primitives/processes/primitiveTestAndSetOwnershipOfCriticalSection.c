/* Extracted from interp.c:25137 (function
 * primitiveTestAndSetOwnershipOfCriticalSection). */

/*	Attempt to test-and-set the ownership of the critical section. If not
        owned, set the owner to the current process and answer false. If owned
   by the current process answer true. If owned by some other process answer
        nil. For simulation if there is an argument it is taken to be the
        effective activeProcess
        (see Process>>effectiveProcess). */

/* InterpreterPrimitives>>#primitiveTestAndSetOwnershipOfCriticalSection */

static void primitiveTestAndSetOwnershipOfCriticalSection(void) {
  sqInt activeProc;
  sqInt criticalSection;
  sqInt objOop;
  sqInt owningProcess;
  sqInt owningProcessIndex;
  char *sp;

  if (argumentCount > 0) {
    /* rcvr */
    criticalSection = longAt(stackPointer + (1 * BytesPerWord));
    activeProc = longAt(stackPointer);
    if (/* isOopForwarded: */
        isOopForwarded(activeProc)) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
    }
  } else {
    /* rcvr */
    criticalSection = longAt(stackPointer);

    /* begin activeProcess */
    objOop =
        longAt((void *)(((longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                                          ((((usqInt)(SchedulerAssociation)
                                             << (shiftForWord()))))))) +
                         BaseHeaderSize) +
                        ((((usqInt)(ValueIndex) << (shiftForWord()))))));
    activeProc = longAt(
        (void *)((objOop + BaseHeaderSize) +
                 ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));
  }

  /* CriticalSections are laid out like Semaphores */
  owningProcessIndex = ExcessSignalsIndex;
  owningProcess =
      longAt((void *)((criticalSection + BaseHeaderSize) +
                      ((((usqInt)(owningProcessIndex) << (shiftForWord()))))));
  if (owningProcess == nilObj) {
    /* begin storePointer:ofObject:withValue: */
    assert(
        validStorePointerArgs(owningProcessIndex, criticalSection, activeProc));
    assert(isNonImmediate(criticalSection));
    if (oopisGreaterThanOrEqualTo(criticalSection, oldSpaceStart)) {
      if (/* isYoung: */
          ((!(activeProc & (tagMask())))) &&
          (oopisLessThan(activeProc, oldSpaceStart))) {
        /* begin possibleRootStoreInto: */
        if (!((byteAt((void *)(criticalSection + (formatFieldByteOffset())))) &
              (1U << (rememberedBitByteShift())))) {
          remember(criticalSection);
        }
      }
    }

    /* most stores into young objects */
    longAtput((void *)((criticalSection + BaseHeaderSize) +
                       ((((usqInt)(owningProcessIndex) << (shiftForWord()))))),
              activeProc);

    /* begin methodReturnValue: */
    assert(!((failed())));
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              falseObj);
    stackPointer = sp;
    return;
  }
  if (owningProcess == activeProc) {
    /* begin methodReturnValue: */
    assert(!((failed())));
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              trueObj);
    stackPointer = sp;
    return;
  }

  /* begin methodReturnValue: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            nilObj);
  stackPointer = sp;
}