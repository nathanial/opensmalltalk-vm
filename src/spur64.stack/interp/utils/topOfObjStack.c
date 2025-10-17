/* Extracted from interp.c:46242 (function topOfObjStack). */

/*	This assert is tricky. push:onObjStack: may call topOfObjStack: just
   after pushing an empty page on the stack, and will ask if the second page is
   valid. */

/* SpurMemoryManager>>#topOfObjStack: */

static sqInt topOfObjStack(sqInt objStack) {
  sqInt topx;

  eassert(isValidObjStackPagemyIndexfirstPage(
      objStack, fetchPointerofObject(ObjStackMyx, objStack),
      objStack ==
          (fetchPointerofObject(fetchPointerofObject(ObjStackMyx, objStack),
                                hiddenRootsObj))));
  topx = fetchPointerofObject(ObjStackTopx, objStack);
  if (!topx) {
    assert((fetchPointerofObject(ObjStackNextx, objStack)) == 0);
    return null;
  }
  return fetchPointerofObject((topx + ObjStackFixedSlots) - 1, objStack);
}