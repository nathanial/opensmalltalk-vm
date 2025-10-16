/* Extracted from interp.c:37814 (function isValidObjStack). */

static NoDbgRegParms sqInt
isValidObjStack(sqInt objStack)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (!((addressCouldBeObj(objStack))
		 && ((numSlotsOfAny(objStack)) == ObjStackPageSlots))) {
		GIV(objStackInvalidBecause) = "first page not obj or wrong size";
		GIV(invalidObjStackPage) = objStack;
		return 0;
	}
	return isValidObjStackPagemyIndexfirstPage(objStack, longAt((void *)((objStack + BaseHeaderSize) + ((((usqInt)(ObjStackMyx) << (shiftForWord())))))), 1);
}