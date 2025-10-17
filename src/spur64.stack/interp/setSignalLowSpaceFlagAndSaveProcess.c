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
	GIV(signalLowSpace) = 1;

	/* begin lowSpaceThreshold: */
	GIV(lowSpaceThreshold) = 0;

	/* N.B. The threshold > 0 guard eliminates a warning when
	   self lowSpaceThreshold: 0
	   is inlined into setSignalLowSpaceFlagAndSaveProcess */
	assert(GIV(totalFreeOldSpace) >= GIV(lowSpaceThreshold));
	lastSavedProcess = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ProcessSignalingLowSpace) << (shiftForWord()))))));
	if (lastSavedProcess == GIV(nilObj)) {
		/* begin activeProcess */
		objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
		activeProc = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));

		/* begin splObj:put: */
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(ProcessSignalingLowSpace, GIV(specialObjectsOop), activeProc));
		assert(isNonImmediate(GIV(specialObjectsOop)));
		if (oopisGreaterThanOrEqualTo(GIV(specialObjectsOop), GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(activeProc & (tagMask()))))
			 && (oopisLessThan(activeProc, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(GIV(specialObjectsOop) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(GIV(specialObjectsOop));
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ProcessSignalingLowSpace) << (shiftForWord()))))),activeProc);
	}
	forceInterruptCheck();
}