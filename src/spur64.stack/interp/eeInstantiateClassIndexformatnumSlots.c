/* Extracted from interp.c:34514 (function eeInstantiateClassIndexformatnumSlots). */

/*	Instantiate an instance of a compact class. ee stands for execution engine
	and implies that this allocation will *NOT* cause a GC. N.B. the
	instantiated object
	IS NOT FILLED and must be completed before returning it to Smalltalk.
	Since this
	call is used in routines that do just that we are safe. Break this rule
	and die in GC.
	Result is guaranteed to be young. */

	/* SpurMemoryManager>>#eeInstantiateClassIndex:format:numSlots: */

usqInt
eeInstantiateClassIndexformatnumSlots(sqInt knownClassIndex, sqInt objFormat, sqInt numSlots)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt newObj;
    usqInt numBytes;

	assert((numSlots >= 0)
	 && ((knownClassIndex != 0)
	 && ((knownClassAtIndex(knownClassIndex)) != GIV(nilObj))));
	assert(((objFormat < (firstByteFormat())
			? objFormat
			: objFormat & (byteFormatMask()))) == (instSpecOfClass(knownClassAtIndex(knownClassIndex))));

	/* begin allocateNewSpaceSlots:format:classIndex: */
	if (numSlots >= (numSlotsMask())) {
		if (numSlots > 0xFFFFFFFFU) {
			return null;
		}
		newObj = GIV(freeStart) + BaseHeaderSize;
		numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
	}
	else {
		newObj = GIV(freeStart);
		numBytes = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
	}
	if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
		if (!GIV(needGCFlag)) {
			/* begin scheduleScavenge */
			GIV(needGCFlag) = 1;
			forceInterruptCheck();
		}
		if ((GIV(freeStart) + numBytes) > (((GIV(eden)).limit))) {
			error("no room in eden for allocateNewSpaceSlots:format:classIndex:");
			return 0;
		}
	}
	if (numSlots >= (numSlotsMask())) {
		longAtput((void *)(GIV(freeStart)),numSlots);
		longAtput((void *)(GIV(freeStart) + 4),((sqInt)((usqInt)((numSlotsMask())) << (numSlotsHalfShift()))));
		long64Atput((void *)(newObj),((((((usqLong) (numSlotsMask()))) << (numSlotsFullShift()))) + ((((usqInt)(objFormat) << (formatShift()))))) + knownClassIndex);
	}
	else {
		long64Atput((void *)(newObj),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(objFormat) << (formatShift()))))) + knownClassIndex);
	}

	/* for header parsing we put a saturated slot count in the prepended overflow size word */
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	GIV(freeStart) += numBytes;
	return newObj;
}