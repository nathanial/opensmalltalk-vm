/* Extracted from interp.c:35050 (function fetchClassTagOfNonImm). */

static NoDbgRegParms sqInt
fetchClassTagOfNonImm(sqInt obj)
{
	return (longAt((void *)(obj))) & (classIndexMask());
}