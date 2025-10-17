/* Extracted from interp.c:37418 (function isInPastSpace). */

	/* SpurMemoryManager>>#isInPastSpace: */

static NoDbgRegParms sqInt
isInPastSpace(sqInt address)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return oopisGreaterThanOrEqualToandLessThan(address, ((pastSpace).start), pastSpaceStart);
}