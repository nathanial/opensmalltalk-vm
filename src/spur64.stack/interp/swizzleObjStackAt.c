/* Extracted from interp.c:46159 (function swizzleObjStackAt). */

/*	On load, swizzle the pointers in an obj stack. Answer the obj stack's oop. */

	/* SpurMemoryManager>>#swizzleObjStackAt: */

static NoDbgRegParms sqInt
swizzleObjStackAt(sqInt objStackRootIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt field;
    sqInt firstPage;
    sqInt index;
    sqInt page;
    sqInt stackOrNil;

	firstPage = (stackOrNil = longAt((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord())))))));
	if (stackOrNil == nilObj) {
		return stackOrNil;
	}
	do {
		assert((numSlotsOfAny(stackOrNil)) == ObjStackPageSlots);
		assert((fetchPointerofObject(ObjStackMyx, stackOrNil)) == objStackRootIndex);

		/* There are four fixed slots in an obj stack, and a Topx of 0 indicates empty, so
		   if there were 5 slots in an oop stack, full would be 2, and the last 0-rel index is 4.
		   Hence the last index is topx + fixed slots - 1, or topx + ObjStackNextx */
		index = (longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) + ObjStackNextx;

		/* swizzle fields including ObjStackNextx, excluding ObjStackFreex and leave field containing the next link. */
		do {
			field = longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
			if (!((field == 0)
				 || (((field & (tagMask())) != 0)))) {
				field = swizzleObj(field);

				/* begin storePointer:ofObjStack:withValue: */
				assert((formatOf(stackOrNil)) == (wordIndexableFormat()));
				longAtput((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))),field);
			}
		} while(((index -= 1)) >= ObjStackNextx);
	} while(((stackOrNil = field)) != 0);
	if ((stackOrNil = longAt((void *)((firstPage + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord())))))))) {
		page = firstPage;
		do {
			stackOrNil = swizzleObj(stackOrNil);

			/* begin storePointer:ofObjStack:withValue: */
			assert((formatOf(page)) == (wordIndexableFormat()));
			longAtput((void *)((page + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),stackOrNil);
			page = stackOrNil;
		} while(((stackOrNil = longAt((void *)((page + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord())))))))) != 0);
	}
	assert(isValidObjStackAt(objStackRootIndex));
	return longAt((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord()))))));
}