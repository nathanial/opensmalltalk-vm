/* Extracted from interp.c:31094 (function allUnscannedEphemeronsAreActive). */

	/* SpurMemoryManager>>#allUnscannedEphemeronsAreActive */

static sqInt
allUnscannedEphemeronsAreActive(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt key;
    usqInt p;

	for (p = ((unscannedEphemerons.start)); p <= ((unscannedEphemerons.top)); p += BytesPerOop) {
		/* begin keyOfMaybeFiredEphemeron: */
		assert((isNonImmediate(longAt((void *)(p))))
		 && (isMaybeFiredEphemeron(longAt((void *)(p)))));
		key = longAt((void *)(((longAt((void *)(p))) + BaseHeaderSize) + (0U << (shiftForWord()))));
		if ((((key & (tagMask())) != 0))
		 || (((byteAt((void *)(key + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)) {
			return 0;
		}
	}
	return 1;
}