/* Extracted from interp.c:63209 (function shortPrintFramesInPage). */

void
shortPrintFramesInPage(StackPage *thePage)
{
	printFrameAndCallersSPshort((thePage->headFP), (thePage->headSP), 1);
}