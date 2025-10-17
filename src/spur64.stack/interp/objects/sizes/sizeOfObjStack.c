/* Extracted from interp.c:45600 (function sizeOfObjStack). */

/* SpurMemoryManager>>#sizeOfObjStack: */

static sqInt sizeOfObjStack(sqInt objStack) {
  sqInt objStackPage;
  sqInt total;

  if (objStack == nilObj) {
    return 0;
  }
  total = longAt((void *)((objStack + BaseHeaderSize) +
                          ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
  objStackPage = objStack;
  while (1) {
    objStackPage =
        longAt((void *)((objStackPage + BaseHeaderSize) +
                        ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
    if (!(objStackPage != 0))
      break;
    total += ObjStackLimit;
    assert((fetchPointerofObject(ObjStackTopx, objStackPage)) == ObjStackLimit);
  }
  return total;
}