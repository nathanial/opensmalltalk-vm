/* Extracted from interp.c:54038 (function frameReceiver). */

static NoDbgRegParms sqInt
frameReceiver(char *theFP)
{
	return longAt(theFP + FoxReceiver);
}