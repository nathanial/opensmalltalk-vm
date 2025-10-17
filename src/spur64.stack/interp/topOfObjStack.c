/* Extracted from interp.c:46242 (function topOfObjStack). */

/*	This assert is tricky. push:onObjStack: may call topOfObjStack: just
   after pushing an empty page on the stack, and will ask if the second page is
   valid. */

/* SpurMemoryManager>>#topOfObjStack: */

static NoDbgRegParms sqInt topOfObjStack(sqInt objStack) {
  sqInt topx;

  eassert(isValidObjStackPagemyIndexfirstPage(
      objStack, fetchPointerofObject(ObjStackMyx, objStack),
      objStack ==
          (fetchPointerofObject(fetchPointerofObject(ObjStackMyx, objStack),
                                hiddenRootsObj))));
  topx = longAt((void *)((objStack + BaseHeaderSize) +
                         ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
  if (!topx) {
    assert((fetchPointerofObject(ObjStackNextx, objStack)) == 0);
    return null;
  }
  return longAt((void *)((objStack + BaseHeaderSize) +
                         ((((usqInt)(((topx + ObjStackFixedSlots) - 1))
                            << (shiftForWord()))))));
}