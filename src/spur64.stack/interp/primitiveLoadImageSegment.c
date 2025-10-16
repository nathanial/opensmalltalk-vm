/* Extracted from interp.c:18942 (function primitiveLoadImageSegment). */

static void
primitiveLoadImageSegment(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt outPointerArray;
    sqInt result;
    sqInt segmentWordArray;
    char *sp;

	outPointerArray = longAt(GIV(stackPointer));
	segmentWordArray = longAt(GIV(stackPointer) + (1 * BytesPerWord));

	/* Essential type checks */
	if (!((/* isArray: */
			((!(outPointerArray & (tagMask()))))
		 && (((byteAt((void *)(outPointerArray + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))
		 && (/* isWords: */
			((!(segmentWordArray & (tagMask()))))
		 && (((((byteAt((void *)(segmentWordArray + (formatFieldByteOffset())))) & (formatMask())) >= (firstLongFormat())) && (((byteAt((void *)(segmentWordArray + (formatFieldByteOffset())))) & (formatMask())) <= ((firstShortFormat()) - 1))))))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}

	/* Must be indexable pointers
	   Must be indexable words
	   the engine returns the roots array which was first in the segment, or an error code on failure. */
	result = loadImageSegmentFromoutPointers(segmentWordArray, outPointerArray);
	if (oopisGreaterThan(result, segmentWordArray)) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (2 * BytesPerWord)),result);
		GIV(stackPointer) = sp;
	}
	else {
		/* primitiveFailFor: */
		GIV(primFailCode) = result;
	}
}