/* Extracted from interp.c:58144 (function numSlotsOfMarriedContext). */

static NoDbgRegParms usqInt
numSlotsOfMarriedContext(sqInt aContext)
{
    usqInt contextSize;

	contextSize = stackPointerIndexForFrame(frameOfMarriedContext(aContext));
	assert((CtxtTempFrameStart + contextSize) <= (numSlotsOf(aContext)));
	return CtxtTempFrameStart + contextSize;
}