/* Extracted from interp.c:45600 (function sizeOfObjStack). */

static NoDbgRegParms sqInt
sizeOfObjStack(sqInt objStack)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt objStackPage;
    sqInt total;

	if (objStack == GIV(nilObj)) {
		return 0;
	}
	total = longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
	objStackPage = objStack;
	while (1) {
		objStackPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
		if (!(objStackPage != 0)) break;
		total += ObjStackLimit;
		assert((fetchPointerofObject(ObjStackTopx, objStackPage)) == ObjStackLimit);
	}
	return total;
}