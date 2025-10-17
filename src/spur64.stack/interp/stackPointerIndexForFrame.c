/* Extracted from interp.c:64399 (function stackPointerIndexForFrame). */

/*	Return the 0-based index rel to the given frame.
        (This is what stackPointer used to be before conversion to pointer) */
/*	In the StackInterpreter stacks grow down. */

/* StackInterpreter>>#stackPointerIndexForFrame: */

static usqInt stackPointerIndexForFrame(char *theFP) {
  StackPage *thePage;
  char *theSP;

  /* begin stackPageFor: */
  thePage = stackPageAtpages(
      pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage),
      pages);
  theSP = findSPOfon(theFP, thePage);
  return (((usqInt)(((theFP + FoxReceiver) - theSP))) >> (shiftForWord())) +
         (byteAt((theFP + FoxFrameFlags) + 1));
}