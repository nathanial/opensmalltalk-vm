/* Extracted from interp.c:50217 (function backupContexttoBlockingSendTo). */

/*	Support for primitiveSuspend.
        Assume suspendedContext is that of a process waiting on a condition
        variable. Backup the PC of suspendedContext to the send that entered the
        wait state.
        primitiveEnterCriticalSection pushes false for blocked waiters. false
   must be replaced by the condition variable. */

/* StackInterpreter>>#backupContext:toBlockingSendTo: */

static void backupContexttoBlockingSendTo(sqInt suspendedContext,
                                          sqInt conditionVariable) {
  sqInt pc;
  sqInt senderOop;
  sqInt sp;
  char *theFP;
  sqInt theIP;
  sqInt theMethod;
  sqInt theNewIP;
  StackPage *thePage;

  assert(isContext(suspendedContext));
  theMethod = fetchPointerofObject(MethodIndex, suspendedContext);
  if ((!((fetchPointerofObject(SenderIndex, suspendedContext)) &
         (tagMask())))) {
    pc = fetchPointerofObject(InstructionPointerIndex, suspendedContext);
    sp =
        fetchPointerofObject(StackPointerIndex, suspendedContext);
    assert(((((pc) & 7) == 1)) && (((pc >> 3)) > 0));
    assert(((((sp) & 7) == 1)) && (((sp >> 3)) > 0));
    theIP = ((theMethod + BaseHeaderSize) + ((pc >> 3))) - 1;
    theNewIP = pcPreviousToFunction(theIP, theMethod);
    assert((theNewIP < theIP) && ((theIP - theNewIP) <= 3));
    pc = ((theNewIP - theMethod) - BaseHeaderSize) + 1;

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(suspendedContext)) &&
           (!(isForwarded(suspendedContext))));
    assert(validStorePointerUncheckedArgs(
        InstructionPointerIndex, suspendedContext, (((usqInt)pc << 3) | 1)));
    longAtput(
        (void *)((suspendedContext + BaseHeaderSize) +
                 ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),
        (((usqInt)pc << 3) | 1));

    /* implicitly converts to 0 relative */
    sp = ((sp >> 3)) + ReceiverIndex;
    assert(((fetchPointerofObject(sp, suspendedContext)) == (falseObject())) ||
           ((fetchPointerofObject(sp, suspendedContext)) == conditionVariable));

    /* begin storePointer:ofObject:withValue: */
    assert(validStorePointerArgs(sp, suspendedContext, conditionVariable));
    assert(isNonImmediate(suspendedContext));
    if (oopisGreaterThanOrEqualTo(suspendedContext, oldSpaceStart)) {
      if (/* isYoung: */
          ((!(conditionVariable & (tagMask())))) &&
          (oopisLessThan(conditionVariable, oldSpaceStart))) {
        /* begin possibleRootStoreInto: */
        if (!((byteAt((void *)(suspendedContext + (formatFieldByteOffset())))) &
              (1U << (rememberedBitByteShift())))) {
          remember(suspendedContext);
        }
      }
    }

    /* most stores into young objects */
    longAtput((void *)((suspendedContext + BaseHeaderSize) +
                       ((((usqInt)(sp) << (shiftForWord()))))),
              conditionVariable);
    return;
  }
  assert(isMarriedOrWidowedContext(suspendedContext));
  assert(!((isWidowedContextNoConvert(suspendedContext))));

  /* begin frameOfMarriedContext: */
  senderOop = fetchPointerofObject(SenderIndex, suspendedContext);
  assert((((senderOop) & 7) == 1));
  theFP = ((char *)(senderOop - (smallIntegerTag())));

  /* begin stackPageFor: */
  thePage = stackPageAtpages(
      pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage),
      pages);
  assert(!((thePage == stackPage)));
  assert(theFP == ((thePage->headFP)));
  theIP = (longAt((thePage->headSP))) + 1;

  /* fetchByte uses pre-increment; must + 1 to point at correct bytecode... */
  theNewIP = pcPreviousToFunction(theIP, theMethod);
  assert((theNewIP < theIP) && ((theIP - theNewIP) <= 3));
  longAtput((thePage->headSP), theNewIP - 1);
  assert(((longAt(((thePage->headSP)) + BytesPerWord)) == (falseObject())) ||
         ((longAt(((thePage->headSP)) + BytesPerWord)) == conditionVariable));
  longAtput(((thePage->headSP)) + BytesPerWord, conditionVariable);
}