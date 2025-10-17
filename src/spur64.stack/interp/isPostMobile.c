/* Extracted from interp.c:47715 (function isPostMobile). */

/*	For asserts */

	/* SpurPlanningCompactor>>#isPostMobile: */

static NoDbgRegParms sqInt
isPostMobile(sqInt obj)
{
	return oopisGreaterThanOrEqualToandLessThanOrEqualTo(obj, mobileStart, lastMobileObject);
}