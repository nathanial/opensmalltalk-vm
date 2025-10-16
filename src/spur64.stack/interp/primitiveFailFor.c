/* Extracted from interp.c:15376 (function primitiveFailFor). */

sqInt
primitiveFailFor(sqInt reasonCode)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (GIV(primFailCode) = reasonCode);
}