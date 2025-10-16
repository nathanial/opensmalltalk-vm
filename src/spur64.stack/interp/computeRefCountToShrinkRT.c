/* Extracted from interp.c:27909 (function computeRefCountToShrinkRT). */

static NeverInline void
computeRefCountToShrinkRT(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt count;
    sqInt elephant;
    sqInt entirePopulation;
    int i;
    sqInt iSqInt;
    sqInt j;
    long population[MaxRTRefCount + 1];
    sqInt refCount;
    sqInt referent;
    sqInt referentSqInt;
    sqInt toDoLimit;

	memset(population, 0, (sizeof(long)) * (MaxRTRefCount + 1));
	assert(allNewSpaceObjectsHaveZeroRTRefCount());

	/* begin referenceCountRememberedReferents: */
	for (iSqInt = 0; iSqInt < GIV(rememberedSetSize); iSqInt += 1) {
		elephant = GIV(rememberedSet)[iSqInt];
		if ((!((longAt((void *)(elephant))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			/* begin followForwarded: */
			assert(isUnambiguouslyForwarder(elephant));
			referentSqInt = longAt((void *)((elephant + BaseHeaderSize) + (0U << (shiftForWord()))));
			while (/* isOopForwarded: */
				((!(referentSqInt & (tagMask()))))
			 && ((!((longAt((void *)(referentSqInt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) + (0U << (shiftForWord()))));
			}
			elephant = referentSqInt;
			if (((elephant & (tagMask())) != 0)) {
				elephant = GIV(nilObj);
			}

			/* take care if elephant forwarded to an immediate */
			GIV(rememberedSet)[iSqInt] = elephant;
		}
		toDoLimit = (numPointerSlotsOf(elephant)) - 1;
		for (j = 0; j <= toDoLimit; j += 1) {
			referent = longAt((void *)((elephant + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))));
			if (/* isReallyYoung: */
				((!(referent & (tagMask()))))
			 && ((/* begin isReallyYoungObject: *//* begin isYoungObject: */
				assert(isNonImmediate(referent)),
			oopisLessThan(referent, GIV(oldSpaceStart))))) {
				refCount = ((usqInt)((byteAt((void *)(referent + (formatFieldByteOffset())))))) >> (rememberedBitByteShift());
				if (refCount < MaxRTRefCount) {
					if (refCount > 0) {
						population[refCount] = ((population[refCount]) - 1);
					}
					refCount += 1;

					/* begin rtRefCountOf:put: */
					assert(isYoungObject(referent));
					byteAtput((void *)(referent + (formatFieldByteOffset())),((byteAt((void *)(referent + (formatFieldByteOffset())))) & (formatMask())) + ((((usqInt)(refCount) << (rememberedBitByteShift())))));
					population[refCount] = ((population[refCount]) + 1);
				}
			}
		}
	}

	/* begin setRefCountToShrinkRT: */
	assert((population[0]) == 0);
	entirePopulation = 0;
	for (j = 1; j <= MaxRTRefCount; j += 1) {
		entirePopulation += population[j];
	}
	count = 0;
	i = MaxRTRefCount + 1;
	while ((count < (entirePopulation / 2))
	 && (((i -= 1)) >= 0)) {
		count += population[i];
	}
	GIV(refCountToShrinkRT) = ((i < 0) ? 0 : i);
}