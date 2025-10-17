/* Extracted from interp.c:34653 (function ensureRoomOnObjStackAt). */

/*	An obj stack is a stack of objects stored in a hidden root slot, such as
	the markStack or the ephemeronQueue. It is a linked list of segments,
	with the hot end at the head of the list. It is a word object. The stack
	pointer is in ObjStackTopx and 0 means empty. The list goes through
	ObjStackNextx. We don't want to shrink objStacks, since they're used
	in GC and its good to keep their memory around. So unused pages
	created by popping emptying pages are kept on the ObjStackFreex list. */

	/* SpurMemoryManager>>#ensureRoomOnObjStackAt: */

static NoDbgRegParms sqInt
ensureRoomOnObjStackAt(sqInt objStackRootIndex)
{
    sqInt freeOrNewPage;
    sqInt stackOrNil;

	stackOrNil = longAt((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord()))))));
	if ((stackOrNil == nilObj)
	 || ((longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) >= ObjStackLimit)) {
		freeOrNewPage = (stackOrNil == nilObj
					? 0
					: longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord())))))));
		if (freeOrNewPage) {
			/* begin storePointer:ofObjStack:withValue: */
			assert((formatOf(stackOrNil)) == (wordIndexableFormat()));
			longAtput((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),0);
			assert((!marking)
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
			if (marking) {
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
		longAtput((void *)((freeOrNewPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))),(stackOrNil == nilObj
				? 0
				: stackOrNil));

		/* begin storePointer:ofObjStack:withValue: */
		assert((formatOf(freeOrNewPage)) == (wordIndexableFormat()));
		longAtput((void *)((freeOrNewPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))),0);

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(objStackRootIndex, hiddenRootsObj, freeOrNewPage));
		assert(isNonImmediate(hiddenRootsObj));
		if (oopisGreaterThanOrEqualTo(hiddenRootsObj, oldSpaceStart)) {
			if (/* isYoung: */
				((!(freeOrNewPage & (tagMask()))))
			 && (oopisLessThan(freeOrNewPage, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(hiddenRootsObj + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(hiddenRootsObj);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord()))))),freeOrNewPage);
		assert(isValidObjStackAt(objStackRootIndex));

		/* Added a new page; now update and answer the relevant cached first page. */

		/* begin updateRootOfObjStackAt:with: */
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(objStackRootIndex, hiddenRootsObj, freeOrNewPage));
		assert(isNonImmediate(hiddenRootsObj));
		if (oopisGreaterThanOrEqualTo(hiddenRootsObj, oldSpaceStart)) {
			if (/* isYoung: */
				((!(freeOrNewPage & (tagMask()))))
			 && (oopisLessThan(freeOrNewPage, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(hiddenRootsObj + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(hiddenRootsObj);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord()))))),freeOrNewPage);
		switch (objStackRootIndex) {
		case MarkStackRootIndex:
			markStack = freeOrNewPage;
			break;
		case WeaklingStackRootIndex:
			weaklingStack = freeOrNewPage;
			break;
		case MournQueueRootIndex:
			mournQueue = freeOrNewPage;
			break;
		default:
			error("Case not found and no otherwise clause");
		}
		stackOrNil = freeOrNewPage;
	}
	assert(isValidObjStackAt(objStackRootIndex));
	return stackOrNil;
}