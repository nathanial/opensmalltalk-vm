/* Extracted from interp.c:56720 (function makeContextSnapshotSafe). */

/*	Convert married contexts to widowed contexts, and in the CoInterpreter,
        map any machine code pcs to bytecode pcs, and then nil all slots beyond
        top of stack, so that the context can be resumed on any VM. */

/* StackInterpreter>>#makeContextSnapshotSafe: */

static void makeContextSnapshotSafe(sqInt ctxt) {
  sqInt i;
  sqInt toDoLimit;
  sqInt toDoStart;

  assert(isContext(ctxt));

  /* The stack pages have already been discarded.  Any remaining married
   * contexts are actually widows. */
  if (((((fetchPointerofObject(SenderIndex, ctxt))) & 7) == 1)) {
    /* begin markContextAsDead: */
    assert(isContext(ctxt));
    assert((isNonImmediate(ctxt)) && (!(isForwarded(ctxt))));
    assert(validStorePointerUncheckedArgs(SenderIndex, ctxt, nilObj));
    longAtput((void *)((ctxt + BaseHeaderSize) +
                       ((((usqInt)(SenderIndex) << (shiftForWord()))))),
              nilObj);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(ctxt)) && (!(isForwarded(ctxt))));
    assert(
        validStorePointerUncheckedArgs(InstructionPointerIndex, ctxt, nilObj));
    longAtput(
        (void *)((ctxt + BaseHeaderSize) +
                 ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),
        nilObj);
  } else {
  }

  /* Fill slots beyond top of stack with nil */
  toDoStart = (fetchStackPointerOf(ctxt)) + CtxtTempFrameStart;
  toDoLimit = ((numSlotsOf(ctxt))) - 1;
  for (i = toDoStart; i <= toDoLimit; i += 1) {
    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(ctxt)) && (!(isForwarded(ctxt))));
    assert(validStorePointerUncheckedArgs(i, ctxt, nilObj));
    longAtput((void *)((ctxt + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              nilObj);
  }
}
