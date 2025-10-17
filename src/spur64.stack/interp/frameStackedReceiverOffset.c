/* Extracted from interp.c:54052 (function frameStackedReceiverOffset). */

/*	Answer the offset in bytes from the the frame pointer to its stacked
	receiver. The receiver of a message send or the closure of a block
	activation is
	always on the stack above any arguments and the frame itself. See the
	diagram in StackInterpreter class>>initializeFrameIndices. */

	/* StackInterpreter>>#frameStackedReceiverOffset: */

static NoDbgRegParms sqInt
frameStackedReceiverOffset(char *theFP)
{
	return (FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord()))));
}