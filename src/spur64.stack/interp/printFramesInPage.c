/* Extracted from interp.c:59756 (function printFramesInPage). */

/*	useful for VM debugging */

/* StackInterpreter>>#printFramesInPage: */

void printFramesInPage(StackPage *thePage) {
  printFrameAndCallersSPshort((thePage->headFP), (thePage->headSP), 0);
}