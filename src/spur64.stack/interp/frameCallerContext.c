/* Extracted from interp.c:53935 (function frameCallerContext). */

static NoDbgRegParms sqInt
frameCallerContext(char *theFP)
{
	assert(isBaseFrame(theFP));
	return longAt(theFP + FoxCallerContext);
}