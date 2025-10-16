/* Extracted from interp.c:50496 (function checkedIntegerValueOf). */

sqInt
checkedIntegerValueOf(sqInt intOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if ((((intOop) & 7) == 1)) {
		return (intOop >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return 0;
	}
}