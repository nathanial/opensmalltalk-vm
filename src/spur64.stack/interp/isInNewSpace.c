/* Extracted from interp.c:37403 (function isInNewSpace). */

	/* SpurMemoryManager>>#isInNewSpace: */

static NoDbgRegParms sqInt
isInNewSpace(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (oopisLessThan(objOop, oldSpaceStart))
	 && (oopisGreaterThanOrEqualTo(objOop, newSpaceStart));
}