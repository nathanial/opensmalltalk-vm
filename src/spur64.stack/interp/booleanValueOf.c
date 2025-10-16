/* Extracted from interp.c:50301 (function booleanValueOf). */

sqInt
booleanValueOf(sqInt obj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (obj == GIV(trueObj)) {
		return 1;
	}
	if (obj == GIV(falseObj)) {
		return 0;
	}

	/* begin success: */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	return null;
}