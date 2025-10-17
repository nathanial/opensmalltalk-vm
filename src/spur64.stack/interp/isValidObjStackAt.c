/* Extracted from interp.c:37691 (function isValidObjStackAt). */

/*	Answer if the obj stack at objStackRootIndex is valid. */

	/* SpurMemoryManager>>#isValidObjStackAt: */

static NoDbgRegParms sqInt
isValidObjStackAt(sqInt objStackRootIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt stackOrNil;

	stackOrNil = longAt((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(objStackRootIndex) << (shiftForWord()))))));
	return (stackOrNil == nilObj)
	 || (isValidObjStackPagemyIndexfirstPage(stackOrNil, objStackRootIndex, 1));
}