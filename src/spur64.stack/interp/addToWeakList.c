/* Extracted from interp.c:27714 (function addToWeakList). */

static NoDbgRegParms NeverInline void
addToWeakList(sqInt weakCorpse)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt weakListOffset;

	weakListOffset = (!(GIV(weakList))
			? 0
			: GIV(weakList));

	/* begin setCorpseOffsetOf:to: */
	assert(isYoung(weakCorpse));
	assert(isForwarded(weakCorpse));
	long64Atput((void *)(weakCorpse),headerForSlotshashformatclassIndex(byteAt((void *)(weakCorpse + (numSlotsFieldByteOffset()))), ((usqInt)(weakListOffset)) >> (formatFieldWidthShift()), weakListOffset & (formatMask()), isForwardedObjectClassIndexPun()));
	GIV(weakList) = (((usqInt)((weakCorpse - GIV(newSpaceStart)))) >> 3 /* shiftForAllocationUnit */) + 1;
	assert((firstCorpse(GIV(weakList))) == weakCorpse);
}