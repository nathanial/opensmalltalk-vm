/* Extracted from interp.c:37206 (function isContextNonImm). */

static NoDbgRegParms int
isContextNonImm(sqInt oop)
{
	return ((longAt((void *)(oop))) & (classIndexMask())) == ClassMethodContextCompactIndex;
}