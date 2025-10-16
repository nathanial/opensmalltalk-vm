/* Extracted from interp.c:37596 (function isReallyYoungObject). */

int
isReallyYoungObject(sqInt obj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin isYoungObject: */
	assert(isNonImmediate(obj));
	return oopisLessThan(obj, GIV(oldSpaceStart));
}