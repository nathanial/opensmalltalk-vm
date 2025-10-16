/* Extracted from interp.c:25137 (function primitiveTestAndSetOwnershipOfCriticalSection). */

static void
primitiveTestAndSetOwnershipOfCriticalSection(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeProc;
    sqInt criticalSection;
    sqInt objOop;
    sqInt owningProcess;
    sqInt owningProcessIndex;
    char *sp;

	if (GIV(argumentCount) > 0) {
		/* rcvr */
		criticalSection = longAt(GIV(stackPointer) + (1 * BytesPerWord));
		activeProc = longAt(GIV(stackPointer));
		if (/* isOopForwarded: */
			((!(activeProc & (tagMask()))))
		 && ((!((longAt((void *)(activeProc))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
		}
	}
	else {
		/* rcvr */
		criticalSection = longAt(GIV(stackPointer));

		/* begin activeProcess */
		objOop = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
		activeProc = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));
	}

	/* CriticalSections are laid out like Semaphores */
	owningProcessIndex = ExcessSignalsIndex;
	owningProcess = longAt((void *)((criticalSection + BaseHeaderSize) + ((((usqInt)(owningProcessIndex) << (shiftForWord()))))));
	if (owningProcess == GIV(nilObj)) {
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(owningProcessIndex, criticalSection, activeProc));
		assert(isNonImmediate(criticalSection));
		if (oopisGreaterThanOrEqualTo(criticalSection, GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(activeProc & (tagMask()))))
			 && (oopisLessThan(activeProc, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(criticalSection + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(criticalSection);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((criticalSection + BaseHeaderSize) + ((((usqInt)(owningProcessIndex) << (shiftForWord()))))),activeProc);

		/* begin methodReturnValue: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),GIV(falseObj));
		GIV(stackPointer) = sp;
		return;
	}
	if (owningProcess == activeProc) {
		/* begin methodReturnValue: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),GIV(trueObj));
		GIV(stackPointer) = sp;
		return;
	}

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),GIV(nilObj));
	GIV(stackPointer) = sp;
}