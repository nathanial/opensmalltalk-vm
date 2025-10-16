/* Extracted from interp.c:54952 (function isBaseFrame). */

static NoDbgRegParms int
isBaseFrame(char *theFP)
{
	return (longAt(theFP + FoxSavedFP)) == 0;
}