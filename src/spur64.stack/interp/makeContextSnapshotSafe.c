/* Extracted from interp.c:56720 (function makeContextSnapshotSafe). */

/*	Convert married contexts to widowed contexts, and in the CoInterpreter,
	map any machine code pcs to bytecode pcs, and then nil all slots beyond
	top of stack, so that the context can be resumed on any VM. */

	/* StackInterpreter>>#makeContextSnapshotSafe: */

static NoDbgRegParms void
makeContextSnapshotSafe(sqInt ctxt)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    usqInt numSlots;
    sqInt toDoLimit;
    sqInt toDoStart;

	assert(isContext(ctxt));

	/* The stack pages have already been discarded.  Any remaining married contexts are actually widows. */
	if (((((longAt((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1)) {
		/* begin markContextAsDead: */
		assert(isContext(ctxt));
		assert((isNonImmediate(ctxt))
		 && (!(isForwarded(ctxt))));
		assert(validStorePointerUncheckedArgs(SenderIndex, ctxt, nilObj));
		longAtput((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),nilObj);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(ctxt))
		 && (!(isForwarded(ctxt))));
		assert(validStorePointerUncheckedArgs(InstructionPointerIndex, ctxt, nilObj));
		longAtput((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),nilObj);
	}
	else {
	}

	/* Fill slots beyond top of stack with nil */
	toDoStart = (fetchStackPointerOf(ctxt)) + CtxtTempFrameStart;
	toDoLimit = ((/* begin numSlotsOf: */
	assert((classIndexOf(ctxt)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(ctxt + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(ctxt - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
	for (i = toDoStart; i <= toDoLimit; i += 1) {
		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(ctxt))
		 && (!(isForwarded(ctxt))));
		assert(validStorePointerUncheckedArgs(i, ctxt, nilObj));
		longAtput((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),nilObj);
	}
}