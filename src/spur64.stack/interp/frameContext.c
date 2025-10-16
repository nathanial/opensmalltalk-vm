/* Extracted from interp.c:53955 (function frameContext). */

static NoDbgRegParms sqInt
frameContext(char *theFP)
{
	return longAt(theFP + FoxThisContext);
}