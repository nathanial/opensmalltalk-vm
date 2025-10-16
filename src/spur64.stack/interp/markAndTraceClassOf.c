/* Extracted from interp.c:39309 (function markAndTraceClassOf). */

static NoDbgRegParms void
markAndTraceClassOf(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classIndex;
    sqInt classObj;
    sqInt classTablePage;
    sqInt fieldIndex;
    sqInt objStack;
    sqInt realClassIndex;

	classIndex = (longAt((void *)(objOop))) & (classIndexMask());

	/* begin classOrNilAtIndex: */
	assert((classIndex <= (tagMask()))
	 || (classIndex >= (arrayClassIndexPun())));
	fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

	/* begin fetchPointer:ofObject: */
	classTablePage = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	if (classTablePage == GIV(nilObj)) {
		classObj = GIV(nilObj);
		goto l1;
	}
	fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);
	classObj = longAt((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	/* end classOrNilAtIndex: */
l1:
	assert(objCouldBeClassObj(classObj));
	realClassIndex = (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask());
	if ((classIndex != realClassIndex)
	 && (classIndex > (lastClassIndexPun()))) {
		/* begin setClassIndexOf:to: */
		assert(((realClassIndex >= 0) && (realClassIndex <= (classIndexMask()))));
		longAtput((void *)(objOop),((longAt((void *)(objOop))) & (~(usqIntptr_t)(classIndexMask()))) + realClassIndex);
	}
	if (!((byteAt((void *)(classObj + (markBitsByteOffset())))) & (1U << (markedBitByteShift())))) {
		/* begin setIsMarkedOf:to: */
		assert(!(isFreeObject(classObj)));
		byteAtput((void *)(classObj + (markBitsByteOffset())),(byteAt((void *)(classObj + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
		markAndTraceClassOf(classObj);
		objStack = GIV(markStack);

		/* begin push:onObjStack: */
		assert(addressCouldBeOop(classObj));
		if (((classObj & (tagMask())) != 0)) {
			assert(objStack == GIV(markStack));
			assert(addressCouldBeObj(topOfObjStack((0 == (fetchPointerofObject(ObjStackTopx, objStack))
					? fetchPointerofObject(ObjStackNextx, objStack)
					: objStack))));
		}
		else {
			assert(!((objStack == GIV(markStack))
			 && (isWeakNonImm(classObj))));

			/* There should only be weaklings on the weaklingStack */
			assert((objStack != GIV(weaklingStack))
			 || (isWeakNonImm(classObj)));
		}

		/* There should be no weaklings on the mark stack. */
		noCheckPushonObjStack(classObj, objStack);
	}
}