/* Extracted from interp.c:51577 (function copyBitsFromtoat). */

sqInt
copyBitsFromtoat(sqInt x0, sqInt x1, sqInt y)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    void *fn;

	fn = ioLoadFunctionFrom("copyBitsFromtoat", "BitBltPlugin");
	if (!fn) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return null;
	}
	return ((sqInt (*)(sqInt, sqInt, sqInt))fn)(x0, x1, y);
}