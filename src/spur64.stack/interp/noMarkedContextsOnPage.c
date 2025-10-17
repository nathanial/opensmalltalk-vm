/* Extracted from interp.c:58122 (function noMarkedContextsOnPage). */

/*	Answer true if there are no marked contexts on thePage. */

/* StackInterpreter>>#noMarkedContextsOnPage: */

static NoDbgRegParms sqInt noMarkedContextsOnPage(StackPage *thePage) {
  char *theFP;

  assert(!(isFree(thePage)));
  theFP = (thePage->headFP);
  do {
    if (byteAt((theFP + FoxFrameFlags) + 2)) {
      assert(isContext(frameContext(theFP)));
      if ((byteAt((void *)((longAt(theFP + FoxThisContext)) +
                           (markBitsByteOffset())))) &
          (1U << (markedBitByteShift()))) {
        return 0;
      }
    }
  } while (((theFP = ((char *)(longAt(theFP + FoxSavedFP))))) != 0);
  return 1;
}