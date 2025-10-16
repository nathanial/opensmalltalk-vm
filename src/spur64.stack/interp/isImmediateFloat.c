/* Extracted from interp.c:27333 (function isImmediateFloat). */

static NoDbgRegParms int
isImmediateFloat(sqInt oop)
{
	return ((oop & (smallFloatTag())) != 0);
}