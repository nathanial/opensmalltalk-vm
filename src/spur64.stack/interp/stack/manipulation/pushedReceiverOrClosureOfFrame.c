/* Extracted from interp.c:61204 (function pushedReceiverOrClosureOfFrame). */

/*	The receiver of a message send or the closure of a block activation is
        always on the stack above any arguments and the frame itself. See the
        diagram in StackInterpreter class>>initializeFrameIndices. */

/* StackInterpreter>>#pushedReceiverOrClosureOfFrame: */

static sqInt pushedReceiverOrClosureOfFrame(char *theFP) {
  return longAt(theFP + ((FoxCallerSavedIP + BytesPerWord) +
                         ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1)))
                            << (shiftForWord()))))));
}