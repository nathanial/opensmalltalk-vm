/* Extracted from interp.c:47715 (function isPostMobile). */

static NoDbgRegParms sqInt
isPostMobile(sqInt obj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return oopisGreaterThanOrEqualToandLessThanOrEqualTo(obj, GIV(mobileStart), GIV(lastMobileObject));
}