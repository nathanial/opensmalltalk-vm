/* Extracted from interp.c:66079 (function fieldOrSenderFPofContext). */

/*	Arrange to answer naked frame pointers for unmarried
	senders to avoid reifying contexts in the search. */

	/* StackInterpreterPrimitives>>#fieldOrSenderFP:ofContext: */

static NoDbgRegParms sqInt
fieldOrSenderFPofContext(sqInt index, sqInt contextObj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt senderOop;
    char *spouseFP;
    sqInt tempIndex;

	tempIndex = index - CtxtTempFrameStart;
	if (!(/* isStillMarriedContext: */
			(((((longAt((void *)((contextObj + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(contextObj))))) {
		if (tempIndex >= (fetchStackPointerOf(contextObj))) {
			return GIV(nilObj);
		}
		else {
			return longAt((void *)((contextObj + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
		}
	}

	/* begin frameOfMarriedContext: */
	senderOop = longAt((void *)((contextObj + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
	assert((((senderOop) & 7) == 1));
	spouseFP = ((char *)(senderOop - (smallIntegerTag())));
	if (tempIndex >= (stackPointerIndexForFrame(spouseFP))) {
		return GIV(nilObj);
	}
	return fieldofFrame(index, spouseFP);
}