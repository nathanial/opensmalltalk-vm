/* Extracted from interp.c:60860 (function printStackCallStackOf). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printStackCallStackOf: */

sqInt
printStackCallStackOf(sqInt aContextOrProcessOrFrame)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt context;
    sqInt senderOop;
    char *theFP;

	if (addressCouldBeObj(aContextOrProcessOrFrame)) {
		if ((((longAt((void *)(aContextOrProcessOrFrame))) & (classIndexMask())) == ClassMethodContextCompactIndex)
		 && (checkIsStillMarriedContextcurrentFP(aContextOrProcessOrFrame, null))) {
			return printStackCallStackOf(((sqInt)(frameOfMarriedContext(aContextOrProcessOrFrame))));
		}
		if (aContextOrProcessOrFrame == (activeProcess())) {
			return printStackCallStackOf(((sqInt)GIV(framePointer)));
		}
		if (couldBeProcess(aContextOrProcessOrFrame)) {
			return printCallStackOf(longAt((void *)((aContextOrProcessOrFrame + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord())))))));
		}
		return null;
	}
	theFP = ((void *)aContextOrProcessOrFrame);
	while (1) {
		context = shortReversePrintFrameAndCallers(theFP);
		if (!((((((longAt((void *)((context + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
			 && ((/* begin frameOfMarriedContext: */
				(senderOop = longAt((void *)((context + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))))),
			assert((((senderOop) & 7) == 1)),
			(theFP = ((char *)(senderOop - (smallIntegerTag())))),
			checkIsStillMarriedContextcurrentFP(context, theFP))))) {
			return null;
		}
	}
	return 0;
}