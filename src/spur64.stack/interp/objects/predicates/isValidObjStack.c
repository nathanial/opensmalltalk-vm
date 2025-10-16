/* Extracted from interp.c:37814 (function isValidObjStack). */

/*	Answer if the obj stack at objStackRootIndex is valid. */

/* SpurMemoryManager>>#isValidObjStack: */

static sqInt isValidObjStack(sqInt objStack) {
  if (!((addressCouldBeObj(objStack)) &&
        ((numSlotsOfAny(objStack)) == ObjStackPageSlots))) {
    objStackInvalidBecause = "first page not obj or wrong size";
    invalidObjStackPage = objStack;
    return 0;
  }
  return isValidObjStackPagemyIndexfirstPage(
      objStack, fetchPointerofObject(ObjStackMyx, objStack), 1);
}