/* Extracted from interp.c:51334 (function checkStackPointerForMaybeMarriedContext). */

static NoDbgRegParms sqInt
checkStackPointerForMaybeMarriedContext(sqInt aContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt header;
    sqInt methodPointer;
    sqInt sp;
    sqInt spSqInt;

	if (checkIsStillMarriedContextcurrentFP(aContext, GIV(framePointer))) {
		sp = checkStackPointerIndexForFrame(frameOfMarriedContext(aContext));
		assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(aContext)));
		return sp;
	}
	if (((((longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1)) {
		methodPointer = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(MethodIndex) << (shiftForWord()))))));

		/* begin argumentCountOf: */
		/* begin methodHeaderOf: */
		assert(isCompiledMethod(methodPointer));
		header = longAt((void *)((methodPointer + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

		/* begin argumentCountOfMethodHeader: */
		return (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;
	}
	spSqInt = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
	if (!((((spSqInt) & 7) == 1))) {
		return 0;
	}
	assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(aContext)));
	return (spSqInt >> 3);
}