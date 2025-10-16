/* Extracted from interp.c:28697 (function processWeaklings). */

static void
processWeaklings(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt format;
    sqInt i;
    sqInt listOffset;
    sqInt rootObj;
    usqInt weakCorpse;
    sqInt weakObj;

	assert(allWeakSurvivorsOnWeakList());
	i = 0;
	while (i < GIV(rememberedSetSize)) {
		rootObj = GIV(rememberedSet)[i];

		/* begin isWeakNonImm: */
		format = (byteAt((void *)(rootObj + (formatFieldByteOffset())))) & (formatMask());
		if (format == (weakArrayFormat())) {
			if (processWeakSurvivor(rootObj)) {
				i += 1;
			}
			else {
				setIsRememberedOfto(rootObj, 0);
				if ((i + 1) < GIV(rememberedSetSize)) {
					GIV(rememberedSet)[i] = (GIV(rememberedSet)[GIV(rememberedSetSize) - 1]);
				}
				GIV(rememberedSetSize) -= 1;
			}
		}
		else {
			i += 1;
		}
	}
	if (GIV(weakList)) {
		weakCorpse = ((((usqInt)((GIV(weakList) - 1)) << 3 /* shiftForAllocationUnit */))) + GIV(newSpaceStart);
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
			oopisGreaterThanOrEqualTo(weakObj, GIV(oldSpaceStart))))
			 && (!(((byteAt((void *)(weakObj + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))) != 0)))) {
				remember(weakObj);
			}

			/* begin nextCorpseOrNil: */
			assert(isYoung(weakCorpse));
			listOffset = ((((usqInt)(((long32At((void *)(weakCorpse + 4))) & (identityHashHalfWordMask()))) << (formatFieldWidthShift())))) + ((byteAt((void *)(weakCorpse + (formatFieldByteOffset())))) & (formatMask()));
			weakCorpse = ((sqInt) ((listOffset
		? ((((usqInt)((listOffset - 1)) << 3 /* shiftForAllocationUnit */))) + GIV(newSpaceStart)
		: 0)));
		}
		GIV(weakList) = null;
	}
}