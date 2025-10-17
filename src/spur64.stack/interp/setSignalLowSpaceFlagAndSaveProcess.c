/* Extracted from interp.c:63049 (function setSignalLowSpaceFlagAndSaveProcess). */

/*	The low space semaphore is about to be signaled. Set the signalLowSpace
	flag, and force an interrupt check. Save the currently active process in
	the special
	objects array so that the low space handler will be able to determine the
	process that first triggered a low space condition. The image's low space
	handler is expected
	to nil out the special objects array slot when it handles the low space
	condition.  */

	/* StackInterpreter>>#setSignalLowSpaceFlagAndSaveProcess */

static void
setSignalLowSpaceFlagAndSaveProcess(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeProc;
    sqInt lastSavedProcess;
    sqInt objOop;

	/* signalLowSpace: */
	signalLowSpace = 1;

	/* begin lowSpaceThreshold: */
	lowSpaceThreshold = 0;

	/* N.B. The threshold > 0 guard eliminates a warning when
	   self lowSpaceThreshold: 0
	   is inlined into setSignalLowSpaceFlagAndSaveProcess */
	assert(totalFreeOldSpace >= lowSpaceThreshold);
	lastSavedProcess = longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ProcessSignalingLowSpace) << (shiftForWord()))))));
	if (lastSavedProcess == nilObj) {
		/* begin activeProcess */
		objOop = longAt((void *)(((longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
		activeProc = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));

		/* begin splObj:put: */
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(ProcessSignalingLowSpace, specialObjectsOop, activeProc));
		assert(isNonImmediate(specialObjectsOop));
		if (oopisGreaterThanOrEqualTo(specialObjectsOop, oldSpaceStart)) {
			if (/* isYoung: */
				((!(activeProc & (tagMask()))))
			 && (oopisLessThan(activeProc, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(specialObjectsOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(specialObjectsOop);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ProcessSignalingLowSpace) << (shiftForWord()))))),activeProc);
	}
	forceInterruptCheck();
}