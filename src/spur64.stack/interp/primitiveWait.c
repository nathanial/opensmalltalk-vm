/* Extracted from interp.c:25846 (function primitiveWait). */

	/* InterpreterPrimitives>>#primitiveWait */

static void
primitiveWait(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeProc;
    sqInt excessSignals;
    sqInt integerValue;
    sqInt objOop;
    sqInt sema;

	/* rcvr */
	sema = longAt(stackPointer);
	excessSignals = fetchIntegerofObject(ExcessSignalsIndex, sema);
	if (excessSignals > 0) {
		integerValue = excessSignals - 1;

		/* begin storeInteger:ofObject:withValue: */
		if ((((((usqInt)(integerValue)) >> 60) + 1) & 15) <= 1) {
			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(sema))
			 && (!(isForwarded(sema))));
			assert(validStorePointerUncheckedArgs(ExcessSignalsIndex, sema, (((usqInt)integerValue << 3) | 1)));
			longAtput((void *)((sema + BaseHeaderSize) + ((((usqInt)(ExcessSignalsIndex) << (shiftForWord()))))),(((usqInt)integerValue << 3) | 1));
		}
		else {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
		}
	}
	else {
		/* begin activeProcess */
		objOop = longAt((void *)(((longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
		activeProc = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));
		addLastLinktoList(activeProc, sema);

		/* transferToHighestPriorityProcessFrom: */
		transferTofrom(wakeHighestPriority(), CSWait);
	}
}