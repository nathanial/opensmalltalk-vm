/* Extracted from interp.c:59756 (function printFramesInPage). */

void
printFramesInPage(StackPage *thePage)
{
	printFrameAndCallersSPshort((thePage->headFP), (thePage->headSP), 0);
}