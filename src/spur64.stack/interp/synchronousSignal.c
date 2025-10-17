/* Extracted from interp.c:65033 (function synchronousSignal). */

/*	Signal the given semaphore from within the interpreter.
	Answer if the current process was preempted. */

	/* StackInterpreter>>#synchronousSignal: */

static NoDbgRegParms sqInt
synchronousSignal(sqInt aSemaphore)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ctxt;
    sqInt excessSignals;
    sqInt integerValue;
    sqInt proc;
    sqInt referent;

	assert((addressCouldBeObj(aSemaphore))
	 && ((!(isForwarded(aSemaphore)))
	 && ((numSlotsOf(aSemaphore)) > ExcessSignalsIndex)));

	/* begin isEmptyList: */
	assert(!(isForwarded(aSemaphore)));
	if ((longAt((void *)((aSemaphore + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))))) == GIV(nilObj)) {
		excessSignals = fetchIntegerofObject(ExcessSignalsIndex, aSemaphore);
		integerValue = excessSignals + 1;

		/* begin storeInteger:ofObject:withValue: */
		if ((((((usqInt)(integerValue)) >> 60) + 1) & 15) <= 1) {
			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(aSemaphore))
			 && (!(isForwarded(aSemaphore))));
			assert(validStorePointerUncheckedArgs(ExcessSignalsIndex, aSemaphore, (((usqInt)integerValue << 3) | 1)));
			longAtput((void *)((aSemaphore + BaseHeaderSize) + ((((usqInt)(ExcessSignalsIndex) << (shiftForWord()))))),(((usqInt)integerValue << 3) | 1));
		}
		else {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
		}
		return 0;
	}

	/* no process is waiting on this semaphore */

	/* begin ensureSemaphoreUnforwardedThroughContext: */
	proc = longAt((void *)((aSemaphore + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))));
	if ((!((longAt((void *)(proc))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		followForwardedObjectFieldstoDepth(aSemaphore, 1);
		proc = longAt((void *)((aSemaphore + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))));
	}
	assert(!((isForwarded(proc))));
	ctxt = longAt((void *)((proc + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))));
	if ((!((longAt((void *)(ctxt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		/* begin followForwarded: */
		assert(isUnambiguouslyForwarder(ctxt));
		referent = longAt((void *)((ctxt + BaseHeaderSize) + (0U << (shiftForWord()))));
		while (/* isOopForwarded: */
			((!(referent & (tagMask()))))
		 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
		}
		ctxt = referent;

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(SuspendedContextIndex, proc, ctxt));
		assert(isNonImmediate(proc));
		if (oopisGreaterThanOrEqualTo(proc, GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(ctxt & (tagMask()))))
			 && (oopisLessThan(ctxt, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(proc + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(proc);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((proc + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))),ctxt);
	}
	return resumepreemptedYieldingIffrom(removeFirstLinkOfList(aSemaphore), GIV(preemptionYields), CSSynchronousSignal);
}