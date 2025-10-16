/* Extracted from interp.c:52935 (function fetchArrayofObject). */

void *
fetchArrayofObject(sqInt fieldIndex, sqInt objectPointer)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arrayOop;

	arrayOop = longAt((void *)((objectPointer + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));

	/* begin arrayValueOf: */
	if (/* isWordsOrBytes: */
		((!(arrayOop & (tagMask()))))
	 && (isWordsOrBytesNonImm(arrayOop))) {
		return ((void *) (pointerForOop(arrayOop + BaseHeaderSize)));
	}

	/* begin primitiveFail */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	return null;
}