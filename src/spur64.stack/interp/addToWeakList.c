/* Extracted from interp.c:27714 (function addToWeakList). */

/*	weakCorpse is the corpse of a weak array that was copied and forwarded.
	Later on its surviving copy must be scanned to nil weak references.
	Thread the corpse onto the weakList. Later, the weakList can be followed,
	and the forwarding pointer followed to locate the survivor. */
/*	Should be too infrequent to lower icache density of copyAndForward: */

	/* SpurGenerationScavenger>>#addToWeakList: */

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