/* Extracted from interp.c:39380 (function markAndTraceObjStackandContents). */

/*	An obj stack is a stack of objects stored in a hidden root slot, such
	as the markStack or the ephemeronQueue. It is a linked list of
	segments, with the hot end at the head of the list. It is a word object.
	The stack pointer is in ObjStackTopx and 0 means empty. */

	/* SpurMemoryManager>>#markAndTraceObjStack:andContents: */

static NoDbgRegParms void
markAndTraceObjStackandContents(sqInt stackOrNil, sqInt markAndTraceContents)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt field;
    sqInt index;

	if (stackOrNil == GIV(nilObj)) {
		return;
	}

	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(stackOrNil)));
	byteAtput((void *)(stackOrNil + (markBitsByteOffset())),(byteAt((void *)(stackOrNil + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
	assert((numSlotsOfAny(stackOrNil)) == ObjStackPageSlots);
	field = longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
	if (field) {
		markAndTraceObjStackandContents(field, markAndTraceContents);
	}
	field = stackOrNil;
	while (1) {
		field = longAt((void *)((field + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))));
		if (!(field != 0)) break;
		/* begin setIsMarkedOf:to: */
		assert(!(isFreeObject(field)));
		byteAtput((void *)(field + (markBitsByteOffset())),(byteAt((void *)(field + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
	}
	if (!markAndTraceContents) {
		return;
	}

	/* There are four fixed slots in an obj stack, and a Topx of 0 indicates empty, so
	   if there were 6 slots in an oop stack, full would be 2, and the last 0-rel index is 5. */
	index = (longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) + ObjStackNextx;
	while (index >= ObjStackFixedSlots) {
		/* begin followObjField:ofObject: */
		field = longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
		assert(isNonImmediate(field));
		if ((!((longAt((void *)(field))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			field = fixFollowedFieldofObjectwithInitialValue(index, stackOrNil, field);
		}
		if (!(((field & (tagMask())) != 0))) {
			markAndTrace(field);
		}
		index -= 1;
	}
}