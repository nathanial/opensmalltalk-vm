/* Extracted from interp.c:63209 (function shortPrintFramesInPage). */

/*	useful for VM debugging */

/* StackInterpreter>>#shortPrintFramesInPage: */

void shortPrintFramesInPage(StackPage *thePage) {
  printFrameAndCallersSPshort((thePage->headFP), (thePage->headSP), 1);
}