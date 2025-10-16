/* Extracted from interp.c:53985 (function frameMethodObject). */

static NoDbgRegParms sqInt
frameMethodObject(char *theFP)
{
	return longAt(theFP + FoxMethod);
}