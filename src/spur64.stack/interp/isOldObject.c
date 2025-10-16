/* Extracted from interp.c:37500 (function isOldObject). */

static NoDbgRegParms int
isOldObject(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(isNonImmediate(objOop));
	return oopisGreaterThanOrEqualTo(objOop, GIV(oldSpaceStart));
}