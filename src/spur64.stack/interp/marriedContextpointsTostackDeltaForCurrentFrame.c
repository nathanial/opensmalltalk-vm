/* Extracted from interp.c:57343 (function marriedContextpointsTostackDeltaForCurrentFrame). */

static NoDbgRegParms sqInt
marriedContextpointsTostackDeltaForCurrentFrame(sqInt spouseContext, sqInt anOop, sqInt stackDeltaForCurrentFrame)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *rcvrOffset;
    sqInt senderOop;
    char *theFP;
    StackPage *thePage;
    char *theSP;

	/* begin frameOfMarriedContext: */
	senderOop = longAt((void *)((spouseContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
	assert((((senderOop) & 7) == 1));
	theFP = ((char *)(senderOop - (smallIntegerTag())));
	if (theFP == GIV(framePointer)) {
		theSP = GIV(stackPointer) + (stackDeltaForCurrentFrame * BytesPerWord);
	}
	else {
		/* begin stackPageFor: */
		thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
		theSP = findSPOfon(theFP, thePage);
	}
	if ((((anOop) & 7) == 1)) {
		if ((anOop == ((((usqInt)((((usqInt)(((theFP + FoxReceiver) - theSP))) >> (shiftForWord())) + (byteAt((theFP + FoxFrameFlags) + 1))) << 3) | 1)))
		 || (anOop == (externalInstVarofContext(InstructionPointerIndex, spouseContext)))) {
			return 1;
		}
	}
	else {
		if (anOop == (longAt(theFP + FoxMethod))) {
			return 1;
		}
		if (longAt(theFP + FoxSavedFP)) {
			if (((byteAt(((((char *)(longAt(theFP + FoxSavedFP)))) + FoxFrameFlags) + 2)) != 0)
			 && (anOop == (longAt((((char *)(longAt(theFP + FoxSavedFP)))) + FoxThisContext)))) {
				return 1;
			}
		}
		else {
			if (anOop == ((/* begin frameCallerContext: */
				assert(isBaseFrame(theFP)),
			longAt(theFP + FoxCallerContext)))) {
				return 1;
			}
		}
	}

	/* Check stack and instruction pointer fields.
	   Check method and sender fields, avoiding unnecessarily reifying sender context.
	   Now check receiver, temps and stack contents */
	rcvrOffset = theFP + FoxReceiver;
	while (theSP <= rcvrOffset) {
		if (anOop == (longAt(theSP))) {
			return 1;
		}
		theSP += BytesPerWord;
	}

	/* Finally check stacked receiver (closure field or duplicate of receiver) and arguments */
	theSP = (theFP + FoxCallerSavedIP) + BytesPerWord;
	rcvrOffset = theFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord())))));
	while (theSP <= rcvrOffset) {
		if (anOop == (longAt(theSP))) {
			return 1;
		}
		theSP += BytesPerWord;
	}
	return 0;
}