/* Extracted from interp.c:14958 (function primitiveEnterCriticalSection). */

/*	Attempt to enter a CriticalSection/Mutex. If not owned, set the owner to
        the current
        process and answer false. If owned by the current process answer true.
        Otherwise suspend the process. Answer if the receiver is owned by the
        current process.
        For simulation, if there is an argument it is taken to be the effective
        activeProcess (see Process>>effectiveProcess). */

/* InterpreterPrimitives>>#primitiveEnterCriticalSection */

static void primitiveEnterCriticalSection(void) {
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
    if (isOopForwarded(activeProc)) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
    }
  } else {
    /* rcvr */
    criticalSection = longAt(stackPointer);

    /* begin activeProcess */
    objOop =
        fetchPointerofObject(ValueIndex, fetchPointerofObject(SchedulerAssociation, specialObjectsOop));
    activeProc = fetchPointerofObject(ActiveProcessIndex, objOop);
  }

  /* CriticalSections are laid out like Semaphores */
  owningProcessIndex = ExcessSignalsIndex;
  owningProcess =
      fetchPointerofObject(owningProcessIndex, criticalSection);
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

  /* Arrange to answer false (unowned) when the process is resumed. */

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            falseObj);
  stackPointer = sp;
  addLastLinktoList(activeProc, criticalSection);

  /* transferToHighestPriorityProcessFrom: */
  transferTofrom(wakeHighestPriority(), CSEnterCriticalSection);
}