/* Extracted from interp.c:26278 (function success). */

sqInt
success(sqInt successBoolean)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (!successBoolean) {
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
	return 0;
}