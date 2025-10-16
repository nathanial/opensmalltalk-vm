/* Extracted from interp.c:15230 (function primitiveFail). */

sqInt
primitiveFail(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	return 0;
}