/* Extracted from interp.c:51551 (function copyBits). */

/*	This entry point needs to be implemented for the interpreter proxy.
	Since BitBlt is now a plugin we need to look up BitBltPlugin:=copyBits
	and call it. This entire mechanism should eventually go away and be
	replaced with a dynamic lookup from BitBltPlugin itself but for backward
	compatibility this stub is provided */

	/* StackInterpreter>>#copyBits */

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