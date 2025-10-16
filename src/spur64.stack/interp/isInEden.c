/* Extracted from interp.c:37357 (function isInEden). */

static NoDbgRegParms sqInt
isInEden(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return oopisGreaterThanOrEqualToandLessThan(objOop, ((GIV(eden)).start), GIV(freeStart));
}