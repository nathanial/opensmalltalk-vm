/* Extracted from interp.c:27688 (function addToEphemeronList). */

/*	ephemeronCorpse is the corpse of an ephemeron that was copied and
	forwarded. Later on its surviving copy must be scanned to nil weak
	references. Thread the corpse onto the weakList. Later, the weakList can
	be followed, and
	the forwarding pointer followed to locate the survivor. */
/*	Should be too infrequent to lower icache density of copyAndForward: */

	/* SpurGenerationScavenger>>#addToEphemeronList: */

static NoDbgRegParms NeverInline void
addToEphemeronList(sqInt ephemeronCorpse)
{
    sqInt ephemeronListOffset;

	assert(!(isScavengeSurvivor(keyOfEphemeron(followForwarded(ephemeronCorpse)))));
	ephemeronListOffset = (!(ephemeronList)
			? 0
			: ephemeronList);

	/* begin setCorpseOffsetOf:to: */
	assert(isYoung(ephemeronCorpse));
	assert(isForwarded(ephemeronCorpse));
	long64Atput((void *)(ephemeronCorpse),headerForSlotshashformatclassIndex(byteAt((void *)(ephemeronCorpse + (numSlotsFieldByteOffset()))), ((usqInt)(ephemeronListOffset)) >> (formatFieldWidthShift()), ephemeronListOffset & (formatMask()), isForwardedObjectClassIndexPun()));
	ephemeronList = (((usqInt)((ephemeronCorpse - newSpaceStart))) >> 3 /* shiftForAllocationUnit */) + 1;
	assert((firstCorpse(ephemeronList)) == ephemeronCorpse);
}