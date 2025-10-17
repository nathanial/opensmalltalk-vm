/* Extracted from interp.c:42359 (function popObjStack). */

	/* SpurMemoryManager>>#popObjStack: */

static NoDbgRegParms sqInt
popObjStack(sqInt objStack)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt myx;
    sqInt nextPage;
    sqInt top;
    sqInt topx;

	eassert(isValidObjStack(objStack));
	topx = longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
	if (!topx) {
		assert((fetchPointerofObject(ObjStackNextx, objStack)) == 0);

		/* for debugging markAndTrace: set (MarkStackRecord := OrderedCollection new) */
		return null;
	}
	topx -= 1;
	top = longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)((topx + ObjStackFixedSlots)) << (shiftForWord()))))));
	assert(addressCouldBeOop(top));

	/* for debugging markAndTrace: set (MarkStackRecord := OrderedCollection new) */

	/* begin storePointer:ofObjStack:withValue: */
	assert((formatOf(objStack)) == (wordIndexableFormat()));
	longAtput((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))),topx);
	if ((topx == 0)
	 && (((nextPage = longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord())))))))) != 0)) {
		/* begin storePointer:ofObjStack:withValue: */
		assert((formatOf(nextPage)) == (wordIndexableFormat()));
		longAtput((void *)((nextPage + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),objStack);

		/* begin storePointer:ofObjStack:withValue: */
		assert((formatOf(objStack)) == (wordIndexableFormat()));
		longAtput((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))),0);
		myx = longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackMyx) << (shiftForWord()))))));

		/* begin updateRootOfObjStackAt:with: */
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(myx, GIV(hiddenRootsObj), nextPage));
		assert(isNonImmediate(GIV(hiddenRootsObj)));
		if (oopisGreaterThanOrEqualTo(GIV(hiddenRootsObj), GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(nextPage & (tagMask()))))
			 && (oopisLessThan(nextPage, GIV(oldSpaceStart)))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(GIV(hiddenRootsObj) + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(GIV(hiddenRootsObj));
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(myx) << (shiftForWord()))))),nextPage);
		switch (myx) {
		case MarkStackRootIndex:
			GIV(markStack) = nextPage;
			break;
		case WeaklingStackRootIndex:
			GIV(weaklingStack) = nextPage;
			break;
		case MournQueueRootIndex:
			GIV(mournQueue) = nextPage;
			break;
		default:
			error("Case not found and no otherwise clause");
		}
		eassert(isValidObjStack(nextPage));
	}
	else {
		eassert(isValidObjStack(objStack));
	}
	return top;
}