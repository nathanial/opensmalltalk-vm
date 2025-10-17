/* Extracted from interp.c:28697 (function processWeaklings). */

/*	Go through the remembered set and the weak list, nilling references to
	any objects that didn't survive the scavenge. Read the class comment
	for a more in-depth description of the algorithm. */

	/* SpurGenerationScavenger>>#processWeaklings */

static void
processWeaklings(void)
{
    sqInt format;
    sqInt i;
    sqInt listOffset;
    sqInt rootObj;
    usqInt weakCorpse;
    sqInt weakObj;

	assert(allWeakSurvivorsOnWeakList());
	i = 0;
	while (i < rememberedSetSize) {
		rootObj = rememberedSet[i];

		/* begin isWeakNonImm: */
		format = (byteAt((void *)(rootObj + (formatFieldByteOffset())))) & (formatMask());
		if (format == (weakArrayFormat())) {
			if (processWeakSurvivor(rootObj)) {
				i += 1;
			}
			else {
				setIsRememberedOfto(rootObj, 0);
				if ((i + 1) < rememberedSetSize) {
					rememberedSet[i] = (rememberedSet[rememberedSetSize - 1]);
				}
				rememberedSetSize -= 1;
			}
		}
		else {
			i += 1;
		}
	}
	if (weakList) {
		weakCorpse = ((((usqInt)((weakList - 1)) << 3 /* shiftForAllocationUnit */))) + newSpaceStart;
		while (weakCorpse) {
			assert(isForwarded(weakCorpse));

			/* begin followForwarded: */
			assert(isUnambiguouslyForwarder(weakCorpse));
			weakObj = longAt((void *)((weakCorpse + BaseHeaderSize) + (0U << (shiftForWord()))));
			while (/* isOopForwarded: */
				((!(weakObj & (tagMask()))))
			 && ((!((longAt((void *)(weakObj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				weakObj = longAt((void *)((weakObj + BaseHeaderSize) + (0U << (shiftForWord()))));
			}

			/* weakObj may have been tenured... */
			if ((processWeakSurvivor(weakObj))
			 && (((/* begin isOldObject: */
				assert(isNonImmediate(weakObj)),
			oopisGreaterThanOrEqualTo(weakObj, oldSpaceStart)))
			 && (!(((byteAt((void *)(weakObj + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))) != 0)))) {
				remember(weakObj);
			}

			/* begin nextCorpseOrNil: */
			assert(isYoung(weakCorpse));
			listOffset = ((((usqInt)(((long32At((void *)(weakCorpse + 4))) & (identityHashHalfWordMask()))) << (formatFieldWidthShift())))) + ((byteAt((void *)(weakCorpse + (formatFieldByteOffset())))) & (formatMask()));
			weakCorpse = ((sqInt) ((listOffset
		? ((((usqInt)((listOffset - 1)) << 3 /* shiftForAllocationUnit */))) + newSpaceStart
		: 0)));
		}
		weakList = null;
	}
}