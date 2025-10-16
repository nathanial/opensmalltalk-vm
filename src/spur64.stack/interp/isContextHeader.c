/* Extracted from interp.c:37199 (function isContextHeader). */

static NoDbgRegParms int
isContextHeader(sqInt aHeader)
{
	return (aHeader & (classIndexMask())) == ClassMethodContextCompactIndex;
}