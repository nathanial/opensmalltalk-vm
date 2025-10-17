/* Extracted from interp.c:27993 (function copyAndForwardMourner). */

/*	A special version of copyAndForward: for objects in the mournQueue. If
	we're in the good times tenuring regime then copy to futureSpace,
	otherwise tenure.
	Also, don't repeat any of the ephemeron processing. */

	/* SpurGenerationScavenger>>#copyAndForwardMourner: */

static NoDbgRegParms sqInt
copyAndForwardMourner(sqInt mourner)
{
    usqInt bytesInObj;
    sqInt format;
    sqInt newLocation;
    sqInt newStart;
    sqInt objOop;
    usqInt startOfSurvivor;
    int tenure;

	assert((isInEden(mourner))
	 || (isInPastSpace(mourner)));

	/* cog methods should be excluded. */
	bytesInObj = bytesInBody(mourner);
	format = (byteAt((void *)(mourner + (formatFieldByteOffset())))) & (formatMask());

	/* Allow Slang to inline. */
	tenure = !((tenureCriterion == TenureByAge)
		 && (tenureThreshold == 0));
	if (tenure
	 || ((futureSurvivorStart + bytesInObj) > ((futureSpace.limit)))) {
		newLocation = copyToOldSpacebytesformat(mourner, bytesInObj, format);
	}
	else {
		/* begin copyToFutureSpace:bytes: */
		/* we hope writes are cheap... */
		statSurvivorCount += 1;
		assert((futureSurvivorStart + bytesInObj) <= ((futureSpace.limit)));
		startOfSurvivor = /* startOfObject: */
				((byteAt((void *)(mourner + (numSlotsFieldByteOffset())))) == (numSlotsMask())
					? mourner - BaseHeaderSize
					: mourner);
		newStart = futureSurvivorStart;
		futureSurvivorStart += bytesInObj;
		memcpy(((void *)newStart), ((void *)startOfSurvivor), bytesInObj);
		if (tenureCriterion == TenureToShrinkRT) {
			objOop = newStart + (mourner - startOfSurvivor);

			/* begin rtRefCountOf:put: */
			assert(isYoungObject(objOop));
			byteAtput((void *)(objOop + (formatFieldByteOffset())),((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) + (0U << (rememberedBitByteShift())));
		}
		newLocation = newStart + (mourner - startOfSurvivor);
	}

	/* begin forwardSurvivor:to: */
	assert(isInNewSpace(mourner));
	assert((isInFutureSpace(newLocation))
	 || (isInOldSpace(newLocation)));

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(mourner))
	 && (!(isForwarded(mourner))));
	assert(validStorePointerUncheckedArgs(0, mourner, newLocation));
	longAtput((void *)((mourner + BaseHeaderSize) + (0U << (shiftForWord()))),newLocation);

	/* begin set:classIndexTo:formatTo: */
	assert((((isForwardedObjectClassIndexPun()) >= 0) && ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
	assert((((forwardedFormat()) >= 0) && ((forwardedFormat()) <= (formatMask()))));
	longAtput((void *)(mourner),((longAt((void *)(mourner))) & (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) + (classIndexMask())))) + ((isForwardedObjectClassIndexPun()) + ((((usqInt)((forwardedFormat())) << (formatShift()))))));

	/* if weak or ephemeron add to the relevant list for subsequent scanning. */
	if (format == (weakArrayFormat())) {
		addToWeakList(mourner);
	}
	return newLocation;
}