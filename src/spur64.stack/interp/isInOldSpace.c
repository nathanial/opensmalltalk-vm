/* Extracted from interp.c:37411 (function isInOldSpace). */

	/* SpurMemoryManager>>#isInOldSpace: */

sqInt
isInOldSpace(sqInt address)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return oopisGreaterThanOrEqualToandLessThan(address, oldSpaceStart, endOfMemory);
}