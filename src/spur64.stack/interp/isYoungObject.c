/* Extracted from interp.c:37914 (function isYoungObject). */

int
isYoungObject(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(isNonImmediate(objOop));
	return oopisLessThan(objOop, GIV(oldSpaceStart));
}