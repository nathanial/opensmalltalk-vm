/* Extracted from interp.c:37691 (function isValidObjStackAt). */

static NoDbgRegParms sqInt
isValidObjStackAt(sqInt objStackRootIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt stackOrNil;

	stackOrNil = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord()))))));
	return (stackOrNil == GIV(nilObj))
	 || (isValidObjStackPagemyIndexfirstPage(stackOrNil, objStackRootIndex, 1));
}