/* Extracted from interp.c:28466 (function noUnfiredEphemeronsAtEndOfRememberedSet). */

/*	For assert checking only. */

	/* SpurGenerationScavenger>>#noUnfiredEphemeronsAtEndOfRememberedSet */

static sqInt
noUnfiredEphemeronsAtEndOfRememberedSet(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt format;
    sqInt i;
    sqInt referrer;

	for (i = GIV(numRememberedEphemerons); i < GIV(rememberedSetSize); i += 1) {
		referrer = GIV(rememberedSet)[i];

		/* begin isEphemeron: */
		assert(isNonImmediate(referrer));
		format = (byteAt((void *)(referrer + (formatFieldByteOffset())))) & (formatMask());
		if (format == (ephemeronFormat())) {
			if (!(isScavengeSurvivor(
					(/* begin keyOfEphemeron: */
						assert((isNonImmediate(referrer))
						 && (isObjEphemeron(referrer))),
					/* fetchPointer:ofObject: */
						longAt((void *)((referrer + BaseHeaderSize) + (0U << (shiftForWord())))))))) {
				return 0;
			}
		}
	}
	return 1;
}