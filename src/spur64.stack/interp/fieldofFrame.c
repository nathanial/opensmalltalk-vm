/* Extracted from interp.c:66113 (function fieldofFrame). */

static NoDbgRegParms sqInt
fieldofFrame(sqInt index, char *theFP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *callerFP;
    usqInt frameNumArgs;

	switch (index) {
	case SenderIndex:
		callerFP = ((char *)(longAt(theFP + FoxSavedFP)));
		return (callerFP
				? (byteAt((callerFP + FoxFrameFlags) + 2)
						? (assert(checkIsStillMarriedContextcurrentFP(frameContext(callerFP), null)),
						/* frameContext: */
							longAt(callerFP + FoxThisContext))
						: ((sqInt)callerFP))
				: (/* begin frameCallerContext: */
					assert(isBaseFrame(theFP)),
				longAt(theFP + FoxCallerContext)));

	case StackPointerIndex:
	case InstructionPointerIndex:
		return ConstZero;

	case MethodIndex:
		return longAt(theFP + FoxMethod);

	case ClosureIndex:
		return (byteAt((theFP + FoxFrameFlags) + 3)
				? longAt(theFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord()))))))
				: GIV(nilObj));

	case ReceiverIndex:
		return longAt(theFP + FoxReceiver);

	default:
		assert((((index - CtxtTempFrameStart) >= 0) && ((index - CtxtTempFrameStart) <= (stackPointerIndexForFrame(theFP)))));
		return /* temporary:in: */
			((index - CtxtTempFrameStart) < ((frameNumArgs = byteAt((theFP + FoxFrameFlags) + 1)))
				? longAt((theFP + FoxCallerSavedIP) + ((frameNumArgs - (index - CtxtTempFrameStart)) * BytesPerWord))
				: longAt(((theFP + FoxReceiver) - BytesPerWord) + ((frameNumArgs - (index - CtxtTempFrameStart)) * BytesPerWord)));
	}
	return 0;
}