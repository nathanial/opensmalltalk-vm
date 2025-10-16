/* Extracted from interp.c:53992 (function frameMethod). */

static NoDbgRegParms sqInt
frameMethod(char *theFP)
{
	return longAt(theFP + FoxMethod);
}