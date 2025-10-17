/* Extracted from interp.c:37705 (function isValidObjStackPagemyIndex). */

/*	Just check the page itself. */

	/* SpurMemoryManager>>#isValidObjStackPage:myIndex: */

static NoDbgRegParms sqInt
isValidObjStackPagemyIndex(sqInt objStackPage, sqInt myx)
{
	if (!(((longAt((void *)(objStackPage))) & (classIndexMask())) == (sixtyFourBitLongsClassIndexPun()))) {
		objStackInvalidBecause = "wrong class index";
		invalidObjStackPage = objStackPage;
		return 0;
	}
	if (!(((byteAt((void *)(objStackPage + (formatFieldByteOffset())))) & (formatMask())) == (sixtyFourBitIndexableFormat()))) {
		objStackInvalidBecause = "wrong format";
		invalidObjStackPage = objStackPage;
		return 0;
	}
	if (!((numSlotsOfAny(objStackPage)) == ObjStackPageSlots)) {
		objStackInvalidBecause = "wrong num slots";
		invalidObjStackPage = objStackPage;
		return 0;
	}
	if (!(myx == (longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackMyx) << (shiftForWord()))))))))) {
		objStackInvalidBecause = "wrong myx";
		invalidObjStackPage = objStackPage;
		return 0;
	}
	if (marking
	 && (!(((byteAt((void *)(objStackPage + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))) {
		objStackInvalidBecause = "marking but page is unmarked";
		invalidObjStackPage = objStackPage;
		return 0;
	}
	return 1;
}