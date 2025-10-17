/* Extracted from interp.c:54749 (function
 * ifCurrentStackPageHasValidHeadPointers). */

/*	If thePage is the stackPage and the stackPointer and/or the framePointer
        are pointing within it,
        answer if thePage's heapSP and headFP are equal to the stackPointer and
        framePointer respectively.
 */

/* StackInterpreter>>#ifCurrentStackPageHasValidHeadPointers: */

static NoDbgRegParms sqInt
ifCurrentStackPageHasValidHeadPointers(StackPage *thePage) {
  if (thePage == stackPage) {
    if (/* addressIsInPage: */
        (((thePage->lastAddress)) < framePointer) &&
        (framePointer < ((thePage->baseAddress)))) {
      if (((thePage->headFP)) != framePointer) {
        return 0;
      }
    }
    if (/* addressIsInPage: */
        (((thePage->lastAddress)) < stackPointer) &&
        (stackPointer < ((thePage->baseAddress)))) {
      if (((thePage->headSP)) != stackPointer) {
        return 0;
      }
    }
  }
  return 1;
}