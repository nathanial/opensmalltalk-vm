/* Extracted from interp.c:24187 (function primitiveStoreImageSegment). */

static void
primitiveStoreImageSegment(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arrayOfRoots;
    sqInt ecode;
    sqInt outPointerArray;
    sqInt segmentWordArray;

	outPointerArray = longAt(GIV(stackPointer));
	segmentWordArray = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	arrayOfRoots = longAt(GIV(stackPointer) + (2 * BytesPerWord));

	/* Essential type checks */
	if (!((/* isArray: */
			((!(arrayOfRoots & (tagMask()))))
		 && (((byteAt((void *)(arrayOfRoots + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))
		 && ((/* isArray: */
			((!(outPointerArray & (tagMask()))))
		 && (((byteAt((void *)(outPointerArray + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))
		 && (/* isWords: */
			((!(segmentWordArray & (tagMask()))))
		 && (((((byteAt((void *)(segmentWordArray + (formatFieldByteOffset())))) & (formatMask())) >= (firstLongFormat())) && (((byteAt((void *)(segmentWordArray + (formatFieldByteOffset())))) & (formatMask())) <= ((firstShortFormat()) - 1)))))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}

	/* Must be indexable pointers
	   Must be indexable words */
	ecode = storeImageSegmentIntooutPointersroots(segmentWordArray, outPointerArray, arrayOfRoots);
	if (ecode == PrimErrNeedCompaction) {
		fullGC();
		outPointerArray = longAt(GIV(stackPointer));
		segmentWordArray = longAt(GIV(stackPointer) + (1 * BytesPerWord));
		arrayOfRoots = longAt(GIV(stackPointer) + (2 * BytesPerWord));
		ecode = storeImageSegmentIntooutPointersroots(segmentWordArray, outPointerArray, arrayOfRoots);
	}
	if (ecode) {
		/* primitiveFailFor: */
		GIV(primFailCode) = ecode;
	}
	else {
		/* begin pop: */
		GIV(stackPointer) += 3 * BytesPerWord;
	}
}