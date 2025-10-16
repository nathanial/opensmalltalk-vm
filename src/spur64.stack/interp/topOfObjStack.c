/* Extracted from interp.c:46242 (function topOfObjStack). */

static NoDbgRegParms sqInt
topOfObjStack(sqInt objStack)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt topx;

	eassert(isValidObjStackPagemyIndexfirstPage(objStack, fetchPointerofObject(ObjStackMyx, objStack), objStack == (fetchPointerofObject(fetchPointerofObject(ObjStackMyx, objStack), GIV(hiddenRootsObj)))));
	topx = longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
	if (!topx) {
		assert((fetchPointerofObject(ObjStackNextx, objStack)) == 0);
		return null;
	}
	return longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(((topx + ObjStackFixedSlots) - 1)) << (shiftForWord()))))));
}