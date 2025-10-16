/* Extracted from interp.c:37286 (function isForwardedClassTag). */

static NoDbgRegParms int
isForwardedClassTag(sqInt classIndex)
{
	return classIndex == (isForwardedObjectClassIndexPun());
}