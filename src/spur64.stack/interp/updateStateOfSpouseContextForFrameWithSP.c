/* Extracted from interp.c:65480 (function updateStateOfSpouseContextForFrameWithSP). */

/*	Update the frame's spouse context with the frame's current state except
	for the
	sender and instruction pointer, which are used to mark the context as
	married, and the receiver and method, which are already set. */

	/* StackInterpreter>>#updateStateOfSpouseContextForFrame:WithSP: */

static NoDbgRegParms void
updateStateOfSpouseContextForFrameWithSP(char *theFP, char *theSP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt fieldIndex;
    sqInt i;
    char *pointer;
    usqInt tempIndex;
    sqInt theContext;
    sqInt valuePointer;

	assert(frameHasContext(theFP));
	theContext = longAt(theFP + FoxThisContext);
	assert(frameReceiver(theFP) == noFixupFollowFieldofObject(ReceiverIndex, theContext));
	assert(frameMethodObject(theFP) == noFixupFollowFieldofObject(MethodIndex, theContext));
	tempIndex = byteAt((theFP + FoxFrameFlags) + 1);

	/* update the arguments. this would appear not to be strictly necessary, but is for two reasons.
	   First, the fact that arguments are read-only is only as convention in the Smalltalk compiler;
	   other languages may choose to modify arguments.
	   Second, the Squeak runUntilErrorOrReturnFrom: nightmare pops the stack top, which may, in
	   certain circumstances, be the last argument, and hence the last argument may not have been
	   stored into the context. */
	pointer = theFP + ((FoxCallerSavedIP + BytesPerWord) + ((tempIndex << (shiftForWord()))));
	for (i = 1; i <= tempIndex; i += 1) {
		pointer -= BytesPerWord;
		assert(addressCouldBeOop(longAt(pointer)));
		fieldIndex = ReceiverIndex + i;
		valuePointer = longAt(pointer);

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(fieldIndex, theContext, valuePointer));
		assert(isNonImmediate(theContext));
		if (oopisGreaterThanOrEqualTo(theContext, oldSpaceStart)) {
			if (/* isYoung: */
				((!(valuePointer & (tagMask()))))
			 && (oopisLessThan(valuePointer, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(theContext + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(theContext);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((theContext + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),valuePointer);
	}

	/* now update the non-argument stack contents. */
	pointer = (theFP + FoxReceiver) - BytesPerWord;
	while (pointer >= theSP) {
		assert(addressCouldBeOop(longAt(pointer)));
		tempIndex += 1;
		fieldIndex = ReceiverIndex + tempIndex;
		valuePointer = longAt(pointer);

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(fieldIndex, theContext, valuePointer));
		assert(isNonImmediate(theContext));
		if (oopisGreaterThanOrEqualTo(theContext, oldSpaceStart)) {
			if (/* isYoung: */
				((!(valuePointer & (tagMask()))))
			 && (oopisLessThan(valuePointer, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(theContext + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(theContext);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((theContext + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),valuePointer);
		pointer -= BytesPerWord;
	}
	assert((ReceiverIndex + tempIndex) < (lengthOf(theContext)));

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(theContext))
	 && (!(isForwarded(theContext))));
	assert(validStorePointerUncheckedArgs(StackPointerIndex, theContext, ((tempIndex << 3) | 1)));
	longAtput((void *)((theContext + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))),((tempIndex << 3) | 1));
}