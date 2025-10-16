/* Extracted from interp.c:53004 (function fetchPointerofMarriedContext). */

static NoDbgRegParms sqInt
fetchPointerofMarriedContext(sqInt offset, sqInt aContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *callerFP;
    usqInt frameNumArgs;
    sqInt senderOop;
    char *spouseFP;

	assert(isContext(aContext));
	assert((((GIV(stackPage)->headFP)) == GIV(framePointer))
	 && (((GIV(stackPage)->headSP)) == GIV(stackPointer)));
	assert(checkIsStillMarriedContextcurrentFP(aContext, GIV(framePointer)));

	/* method, closureOrNil & receiver need no special handling; only
	   sender, pc & stackp have to be computed for married contexts. */
	if (offset <= ReceiverIndex) {
		if (!(offset <= StackPointerIndex)) {
			return longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(offset) << (shiftForWord()))))));
		}

		/* begin frameOfMarriedContext: */
		senderOop = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
		assert((((senderOop) & 7) == 1));
		spouseFP = ((char *)(senderOop - (smallIntegerTag())));
		if (!offset) {
			/* begin ensureCallerContext: */
			callerFP = ((char *)(longAt(spouseFP + FoxSavedFP)));
			if (!callerFP) {
				/* begin frameCallerContext: */
				assert(isBaseFrame(spouseFP));
				return longAt(spouseFP + FoxCallerContext);
			}

			/* base frame, context in saved ip slot (or base of stack in Cog) */

			/* begin ensureFrameIsMarried:SP: */
			if (byteAt((callerFP + FoxFrameFlags) + 2)) {
				assert(isContext(frameContext(callerFP)));
				return longAt(callerFP + FoxThisContext);
			}
			return marryFrameSP(
				callerFP,
				(/* begin frameCallerStackPointer: */
					assert(!(isBaseFrame(spouseFP))),
				(spouseFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((spouseFP + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord));
		}
		if (offset == StackPointerIndex) {
			return (((stackPointerIndexForFrame(spouseFP)) << 3) | 1);
		}
		if (offset == InstructionPointerIndex) {
			return instructionPointerForFramecurrentFPcurrentIP(spouseFP, GIV(framePointer), GIV(instructionPointer));
		}
	}

	/* begin frameOfMarriedContext: */
	senderOop = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
	assert((((senderOop) & 7) == 1));
	spouseFP = ((char *)(senderOop - (smallIntegerTag())));
	return ((((offset - ReceiverIndex) >= 1) && ((offset - ReceiverIndex) <= (stackPointerIndexForFrame(spouseFP))))
			? /* temporary:in: */
				((offset - (ReceiverIndex + 1)) < ((frameNumArgs = byteAt((spouseFP + FoxFrameFlags) + 1)))
					? longAt((spouseFP + FoxCallerSavedIP) + ((frameNumArgs - (offset - (ReceiverIndex + 1))) * BytesPerWord))
					: longAt(((spouseFP + FoxReceiver) - BytesPerWord) + ((frameNumArgs - (offset - (ReceiverIndex + 1))) * BytesPerWord)))
			: GIV(nilObj));
}