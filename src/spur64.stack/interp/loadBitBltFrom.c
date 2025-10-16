/* Extracted from interp.c:55585 (function loadBitBltFrom). */

sqInt
loadBitBltFrom(sqInt bb)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    void *fn;

	fn = ioLoadFunctionFrom("loadBitBltFrom", "BitBltPlugin");
	if (!fn) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return null;
	}
	return ((sqInt (*)(sqInt))fn)(bb);
}