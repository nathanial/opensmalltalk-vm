/* Extracted from interp.c:58144 (function numSlotsOfMarriedContext). */

/*	Answer the number of effective pointer fields in the given context. */

	/* StackInterpreter>>#numSlotsOfMarriedContext: */

static NoDbgRegParms usqInt
numSlotsOfMarriedContext(sqInt aContext)
{
    usqInt contextSize;

	contextSize = stackPointerIndexForFrame(frameOfMarriedContext(aContext));
	assert((CtxtTempFrameStart + contextSize) <= (numSlotsOf(aContext)));
	return CtxtTempFrameStart + contextSize;
}