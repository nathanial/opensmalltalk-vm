/* Extracted from interp.c:37705 (function isValidObjStackPagemyIndex). */

static NoDbgRegParms sqInt
isValidObjStackPagemyIndex(sqInt objStackPage, sqInt myx)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (!(((longAt((void *)(objStackPage))) & (classIndexMask())) == (sixtyFourBitLongsClassIndexPun()))) {
		GIV(objStackInvalidBecause) = "wrong class index";
		GIV(invalidObjStackPage) = objStackPage;
		return 0;
	}
	if (!(((byteAt((void *)(objStackPage + (formatFieldByteOffset())))) & (formatMask())) == (sixtyFourBitIndexableFormat()))) {
		GIV(objStackInvalidBecause) = "wrong format";
		GIV(invalidObjStackPage) = objStackPage;
		return 0;
	}
	if (!((numSlotsOfAny(objStackPage)) == ObjStackPageSlots)) {
		GIV(objStackInvalidBecause) = "wrong num slots";
		GIV(invalidObjStackPage) = objStackPage;
		return 0;
	}
	if (!(myx == (longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackMyx) << (shiftForWord()))))))))) {
		GIV(objStackInvalidBecause) = "wrong myx";
		GIV(invalidObjStackPage) = objStackPage;
		return 0;
	}
	if (GIV(marking)
	 && (!(((byteAt((void *)(objStackPage + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))) {
		GIV(objStackInvalidBecause) = "marking but page is unmarked";
		GIV(invalidObjStackPage) = objStackPage;
		return 0;
	}
	return 1;
}