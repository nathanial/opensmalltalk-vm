/* Extracted from interp.c:37364 (function isInFutureSpace). */

static NoDbgRegParms sqInt
isInFutureSpace(sqInt address)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return oopisGreaterThanOrEqualToandLessThan(address, ((GIV(futureSpace)).start), GIV(futureSurvivorStart));
}