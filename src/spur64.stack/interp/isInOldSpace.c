/* Extracted from interp.c:37411 (function isInOldSpace). */

sqInt
isInOldSpace(sqInt address)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return oopisGreaterThanOrEqualToandLessThan(address, GIV(oldSpaceStart), GIV(endOfMemory));
}