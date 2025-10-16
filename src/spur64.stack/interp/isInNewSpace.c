/* Extracted from interp.c:37403 (function isInNewSpace). */

static NoDbgRegParms sqInt
isInNewSpace(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (oopisLessThan(objOop, GIV(oldSpaceStart)))
	 && (oopisGreaterThanOrEqualTo(objOop, GIV(newSpaceStart)));
}