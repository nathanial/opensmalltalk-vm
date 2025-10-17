/* Extracted from interp.c:37814 (function isValidObjStack). */

/*	Answer if the obj stack at objStackRootIndex is valid. */

	/* SpurMemoryManager>>#isValidObjStack: */

static NoDbgRegParms sqInt
isValidObjStack(sqInt objStack)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (!((addressCouldBeObj(objStack))
		 && ((numSlotsOfAny(objStack)) == ObjStackPageSlots))) {
		objStackInvalidBecause = "first page not obj or wrong size";
		invalidObjStackPage = objStack;
		return 0;
	}
	return isValidObjStackPagemyIndexfirstPage(objStack, longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackMyx) << (shiftForWord())))))), 1);
}