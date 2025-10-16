/* Extracted from interp.c:51551 (function copyBits). */

sqInt
copyBits(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    void *fn;

	fn = ioLoadFunctionFrom("copyBits", "BitBltPlugin");
	if (!fn) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return null;
	}
	return ((sqInt (*)(void))fn)();
}