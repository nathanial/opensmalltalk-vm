/* Extracted from interp.c:34653 (function ensureRoomOnObjStackAt). */

static NoDbgRegParms sqInt
ensureRoomOnObjStackAt(sqInt objStackRootIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt freeOrNewPage;
    sqInt stackOrNil;

	stackOrNil = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord()))))));
	if ((stackOrNil == GIV(nilObj))
	 || ((longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) >= ObjStackLimit)) {
		freeOrNewPage = (stackOrNil == GIV(nilObj)
					? 0
					: longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord())))))));
		if (freeOrNewPage) {
			/* begin storePointer:ofObjStack:withValue: */
			assert((formatOf(stackOrNil)) == (wordIndexableFormat()));
			longAtput((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),0);
			assert((!GIV(marking))
			 || (isMarked(freeOrNewPage)));
		}
		else {
			freeOrNewPage = allocateSlotsInOldSpacebytesformatclassIndex(ObjStackPageSlots, ((((usqInt)(ObjStackPageSlots) << (shiftForWord())))) + (BaseHeaderSize + BaseHeaderSize), sixtyFourBitIndexableFormat(), sixtyFourBitLongsClassIndexPun());
			if (!freeOrNewPage) {
				growOldSpaceByAtLeast(ObjStackPageSlots * BytesPerOop);
				freeOrNewPage = allocateSlotsInOldSpacebytesformatclassIndex(ObjStackPageSlots, ((((usqInt)(ObjStackPageSlots) << (shiftForWord())))) + (BaseHeaderSize + BaseHeaderSize), sixtyFourBitIndexableFormat(), sixtyFourBitLongsClassIndexPun());
				if (!freeOrNewPage) {
					error("no memory to allocate or extend obj stack");
				}
			}

			/* Allocate a new segment and retry. This is very uncommon. But it happened to me (Clement). */

			/* begin storePointer:ofObjStack:withValue: */
			assert((formatOf(freeOrNewPage)) == (wordIndexableFormat()));
			longAtput((void *)((freeOrNewPage + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),0);
			if (GIV(marking)) {
				/* begin setIsMarkedOf:to: */
				assert(!(isFreeObject(freeOrNewPage)));
				byteAtput((void *)(freeOrNewPage + (markBitsByteOffset())),(byteAt((void *)(freeOrNewPage + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
			}
		}

		/* the free page list is always on the new page. */

		/* begin storePointer:ofObjStack:withValue: */
		assert((formatOf(freeOrNewPage)) == (wordIndexableFormat()));
		longAtput((void *)((freeOrNewPage + BaseHeaderSize) + ((((usqInt)(ObjStackMyx) << (shiftForWord()))))),objStackRootIndex);

		/* begin storePointer:ofObjStack:withValue: */
		assert((formatOf(freeOrNewPage)) == (wordIndexableFormat()));
		longAtput((void *)((freeOrNewPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))),(stackOrNil == GIV(nilObj)
				? 0
				: stackOrNil));

		/* begin storePointer:ofObjStack:withValue: */
		assert((formatOf(freeOrNewPage)) == (wordIndexableFormat()));
		longAtput((void *)((freeOrNewPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))),0);

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(objStackRootIndex, GIV(hiddenRootsObj), freeOrNewPage));
		assert(isNonImmediate(GIV(hiddenRootsObj)));
		if (oopisGreaterThanOrEqualTo(GIV(hiddenRootsObj), GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(freeOrNewPage & (tagMask()))))
			 && (oopisLessThan(freeOrNewPage, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(GIV(hiddenRootsObj) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(GIV(hiddenRootsObj));
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord()))))),freeOrNewPage);
		assert(isValidObjStackAt(objStackRootIndex));

		/* Added a new page; now update and answer the relevant cached first page. */

		/* begin updateRootOfObjStackAt:with: */
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(objStackRootIndex, GIV(hiddenRootsObj), freeOrNewPage));
		assert(isNonImmediate(GIV(hiddenRootsObj)));
		if (oopisGreaterThanOrEqualTo(GIV(hiddenRootsObj), GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(freeOrNewPage & (tagMask()))))
			 && (oopisLessThan(freeOrNewPage, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(GIV(hiddenRootsObj) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(GIV(hiddenRootsObj));
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord()))))),freeOrNewPage);
		switch (objStackRootIndex) {
		case MarkStackRootIndex:
			GIV(markStack) = freeOrNewPage;
			break;
		case WeaklingStackRootIndex:
			GIV(weaklingStack) = freeOrNewPage;
			break;
		case MournQueueRootIndex:
			GIV(mournQueue) = freeOrNewPage;
			break;
		default:
			error("Case not found and no otherwise clause");
		}
		stackOrNil = freeOrNewPage;
	}
	assert(isValidObjStackAt(objStackRootIndex));
	return stackOrNil;
}