/* Extracted from interp.c:53239 (function findFrameAboveinPage). */

static NoDbgRegParms char *
findFrameAboveinPage(char *theFP, StackPage *thePage)
{
    char *callerFP;
    char *fp;

	callerFP = ((char *) 0);
	fp = (thePage->headFP);
	if (fp == theFP) {
		return 0;
	}
	while (((callerFP = ((char *)(longAt(fp + FoxSavedFP))))) != 0) {
		if (callerFP == theFP) {
			return fp;
		}
		fp = callerFP;
	}
	error("did not find theFP in stack page");
	return 0;
}