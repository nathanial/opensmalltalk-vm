/* Extracted from interp.c:50145 (function arrayValueOf). */

void *
arrayValueOf(sqInt arrayOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (/* isWordsOrBytes: */
		((!(arrayOop & (tagMask()))))
	 && (isWordsOrBytesNonImm(arrayOop))) {
		return ((void *) (pointerForOop(arrayOop + BaseHeaderSize)));
	}

	/* begin primitiveFail */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	return 0;
}