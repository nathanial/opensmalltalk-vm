/* Extracted from interp.c:45178 (function returnrestoringObjectsInupTosavedFirstFields). */

/*	This is part of storeImageSegmentInto:outPointers:roots:. */

	/* SpurMemoryManager>>#return:restoringObjectsIn:upTo:savedFirstFields: */

static NoDbgRegParms sqInt
returnrestoringObjectsInupTosavedFirstFields(sqInt errCode, sqInt reachableObjectsArray, sqInt limitOrTag, sqInt savedFirstFields)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt oop;
    sqInt valuePointer;

	/* begin restoreObjectsIn:upTo:savedFirstFields: */
	numSlots = (limitOrTag == -1
				? (/* begin numSlotsOf: */
					assert((classIndexOf(reachableObjectsArray)) > (isForwardedObjectClassIndexPun())),
				(((numSlotsUsqInt = byteAt((void *)(reachableObjectsArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
							? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(reachableObjectsArray - BaseHeaderSize)))) << 8)))))) >> 8
							: numSlotsUsqInt))
				: limitOrTag);
	for (i = 0; i < numSlots; i += 1) {
		oop = longAt((void *)((reachableObjectsArray + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		valuePointer = longAt((void *)((savedFirstFields + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(oop))
		 && (!(isForwarded(oop))));
		assert(validStorePointerUncheckedArgs(0, oop, valuePointer));
		longAtput((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))),valuePointer);

		/* begin setIsMarkedOf:to: */
		assert(!(isFreeObject(oop)));
		byteAtput((void *)(oop + (markBitsByteOffset())),(byteAt((void *)(oop + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
	}
	if (oopisGreaterThanOrEqualToandLessThan(savedFirstFields, GIV(oldSpaceStart), GIV(endOfMemory))) {
		freeObject(savedFirstFields);
	}
	freeObject(reachableObjectsArray);
	runLeakCheckerFor(GCModeImageSegment);
	assert(allObjectsUnmarked());
	return errCode;
}