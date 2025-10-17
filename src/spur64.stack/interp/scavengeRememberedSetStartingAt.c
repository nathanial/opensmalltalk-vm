/* Extracted from interp.c:29352 (function scavengeRememberedSetStartingAt). */

/*	scavengeRememberedSetStartingAt: n traverses objects in the remembered
	set starting at the nth one. If the object does not refer to any new
	objects, it
	is removed from the set. Otherwise, its new referents are scavenged. Defer
	scavenging ephemerons until after a complete scavenge has been performed,
	so that triggered ephemerons can be fired. Move them to the front of the
	set and count them in numRememberedEphemerons for later scanning. */

	/* SpurGenerationScavenger>>#scavengeRememberedSetStartingAt: */

static NoDbgRegParms void
scavengeRememberedSetStartingAt(sqInt n)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt destIndex;
    sqInt referrer;
    sqInt sourceIndex;

	sourceIndex = (destIndex = n);
	while (sourceIndex < GIV(rememberedSetSize)) {
		referrer = GIV(rememberedSet)[sourceIndex];

		/* Any potential firing ephemerons should not be scanned yet.
		   Move any to the front of the set to save time in later scanning. */
		if ((isEphemeron(referrer))
		 && (!(isScavengeSurvivor(
			(/* begin keyOfEphemeron: */
				assert((isNonImmediate(referrer))
				 && (isObjEphemeron(referrer))),
			/* fetchPointer:ofObject: */
				longAt((void *)((referrer + BaseHeaderSize) + (0U << (shiftForWord()))))))))) {
			assert(destIndex >= GIV(numRememberedEphemerons));
			GIV(rememberedSet)[destIndex] = (GIV(rememberedSet)[GIV(numRememberedEphemerons)]);
			GIV(rememberedSet)[GIV(numRememberedEphemerons)] = referrer;
			GIV(numRememberedEphemerons) += 1;
			destIndex += 1;
		}
		else {
			if (scavengeReferentsOf(referrer)) {
				GIV(rememberedSet)[destIndex] = referrer;
				destIndex += 1;
			}
			else {
				setIsRememberedOfto(referrer, 0);
			}
		}
		sourceIndex += 1;
	}
	GIV(rememberedSetSize) = destIndex;
	assert(noUnfiredEphemeronsAtEndOfRememberedSet());
}