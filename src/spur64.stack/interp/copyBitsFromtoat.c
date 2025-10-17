/* Extracted from interp.c:51577 (function copyBitsFromtoat). */

/*	This entry point needs to be implemented for the interpreter proxy.
	Since BitBlt is now a plugin we need to look up
	BitBltPlugin:=copyBitsFrom:to:at: and call it. This entire mechanism
	should eventually go away and be
	replaced with a dynamic lookup from BitBltPlugin itself but for backward
	compatibility this stub is provided
 */

	/* StackInterpreter>>#copyBitsFrom:to:at: */

sqInt
copyBitsFromtoat(sqInt x0, sqInt x1, sqInt y)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    void *fn;

	fn = ioLoadFunctionFrom("copyBitsFromtoat", "BitBltPlugin");
	if (!fn) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return null;
	}
	return ((sqInt (*)(sqInt, sqInt, sqInt))fn)(x0, x1, y);
}