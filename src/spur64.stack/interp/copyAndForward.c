/* Extracted from interp.c:28070 (function copyAndForward). */

/*	copyAndForward: survivor copies a survivor object either to
	futureSurvivorSpace or, if it is to be promoted, to oldSpace.
	It leaves a forwarding pointer behind. If the object is weak
	then corpse is threaded onto the weakList for later treatment. */

	/* SpurGenerationScavenger>>#copyAndForward: */

static NoDbgRegParms sqInt
copyAndForward(sqInt survivor)
{
    usqInt bytesInObj;
    sqInt format;
    sqInt newLocation;
    sqInt newStart;
    sqInt objOop;
    usqInt startOfSurvivor;

	assert((isInEden(survivor))
	 || (isInPastSpace(survivor)));

	/* cog methods should be excluded. */
	bytesInObj = bytesInBody(survivor);
	format = (byteAt((void *)(survivor + (formatFieldByteOffset())))) & (formatMask());
	if (((futureSurvivorStart + bytesInObj) > ((futureSpace.limit)))
	 || (/* shouldBeTenured: */
		(tenureCriterion == TenureByAge
			? oopisLessThan(survivor, tenureThreshold)
			: (tenureCriterion == TenureToShrinkRT
					? (((usqInt)((byteAt((void *)(survivor + (formatFieldByteOffset())))))) >> (rememberedBitByteShift())) >= refCountToShrinkRT
					: (tenureCriterion == TenureByClass
							? ((longAt((void *)(survivor))) & (classIndexMask())) == tenuringClassIndex
							: 0))))) {
		newLocation = copyToOldSpacebytesformat(survivor, bytesInObj, format);
	}
	else {
		/* begin copyToFutureSpace:bytes: */
		/* we hope writes are cheap... */
		statSurvivorCount += 1;
		assert((futureSurvivorStart + bytesInObj) <= ((futureSpace.limit)));
		startOfSurvivor = /* startOfObject: */
				((byteAt((void *)(survivor + (numSlotsFieldByteOffset())))) == (numSlotsMask())
					? survivor - BaseHeaderSize
					: survivor);
		newStart = futureSurvivorStart;
		futureSurvivorStart += bytesInObj;
		memcpy(((void *)newStart), ((void *)startOfSurvivor), bytesInObj);
		if (tenureCriterion == TenureToShrinkRT) {
			objOop = newStart + (survivor - startOfSurvivor);

			/* begin rtRefCountOf:put: */
			assert(isYoungObject(objOop));
			byteAtput((void *)(objOop + (formatFieldByteOffset())),((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) + (0U << (rememberedBitByteShift())));
		}
		newLocation = newStart + (survivor - startOfSurvivor);
	}

	/* begin forwardSurvivor:to: */
	assert(isInNewSpace(survivor));
	assert((isInFutureSpace(newLocation))
	 || (isInOldSpace(newLocation)));

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(survivor))
	 && (!(isForwarded(survivor))));
	assert(validStorePointerUncheckedArgs(0, survivor, newLocation));
	longAtput((void *)((survivor + BaseHeaderSize) + (0U << (shiftForWord()))),newLocation);

	/* begin set:classIndexTo:formatTo: */
	assert((((isForwardedObjectClassIndexPun()) >= 0) && ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
	assert((((forwardedFormat()) >= 0) && ((forwardedFormat()) <= (formatMask()))));
	longAtput((void *)(survivor),((longAt((void *)(survivor))) & (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) + (classIndexMask())))) + ((isForwardedObjectClassIndexPun()) + ((((usqInt)((forwardedFormat())) << (formatShift()))))));

	/* if weak or ephemeron add to the relevant list for subsequent scanning. */
	if (((format >= (weakArrayFormat())) && (format <= (ephemeronFormat())))) {
		if (format == (weakArrayFormat())) {
			addToWeakList(survivor);
		}
		else {
			assert(isEphemeronFormat(format));
			if (!(isScavengeSurvivor(
					(/* begin keyOfEphemeron: */
						assert((isNonImmediate(newLocation))
						 && (isObjEphemeron(newLocation))),
					/* fetchPointer:ofObject: */
						longAt((void *)((newLocation + BaseHeaderSize) + (0U << (shiftForWord())))))))) {
				addToEphemeronList(survivor);
			}
		}
	}
	return newLocation;
}