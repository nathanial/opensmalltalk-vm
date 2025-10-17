/* Extracted from interp.c:15094 (function primitiveExitCriticalSection). */

/*	Exit the critical section.
	This may change the active process as a result. */

	/* InterpreterPrimitives>>#primitiveExitCriticalSection */

static void
primitiveExitCriticalSection(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt criticalSection;
    sqInt owningProcess;
    sqInt owningProcessIndex;

	/* rcvr */
	criticalSection = longAt(GIV(stackPointer));

	/* CriticalSections are laid out like Semaphores */
	owningProcessIndex = ExcessSignalsIndex;

	/* begin isEmptyList: */
	assert(!(isForwarded(criticalSection)));
	if ((longAt((void *)((criticalSection + BaseHeaderSize) + ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))))) == GIV(nilObj)) {
		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(criticalSection))
		 && (!(isForwarded(criticalSection))));
		assert(validStorePointerUncheckedArgs(owningProcessIndex, criticalSection, GIV(nilObj)));
		longAtput((void *)((criticalSection + BaseHeaderSize) + ((((usqInt)(owningProcessIndex) << (shiftForWord()))))),GIV(nilObj));
	}
	else {
		owningProcess = removeFirstLinkOfList(criticalSection);

		/* store check unnecessary because criticalSection referred to owningProcess
		   via its FirstLinkIndex slot before owningProcess was removed. */

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(criticalSection))
		 && (!(isForwarded(criticalSection))));
		assert(validStorePointerUncheckedArgs(owningProcessIndex, criticalSection, owningProcess));
		longAtput((void *)((criticalSection + BaseHeaderSize) + ((((usqInt)(owningProcessIndex) << (shiftForWord()))))),owningProcess);

		/* Note that resume: isn't fair; it won't suspend the active process.
		   For fairness we must do the equivalent of a primitiveYield, but that
		   may break old code, so we stick with unfair resume:. */
		resumepreemptedYieldingIffrom(owningProcess, GIV(preemptionYields), CSExitCriticalSection);
	}
}