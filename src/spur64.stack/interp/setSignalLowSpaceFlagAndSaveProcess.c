/* Extracted from interp.c:63049 (function setSignalLowSpaceFlagAndSaveProcess). */

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