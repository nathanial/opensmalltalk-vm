/* Extracted from interp.c:37691 (function isValidObjStackAt). */

/*	Answer if the obj stack at objStackRootIndex is valid. */

/* SpurMemoryManager>>#isValidObjStackAt: */

static sqInt isValidObjStackAt(sqInt objStackRootIndex) {
  sqInt stackOrNil;

  stackOrNil =
      fetchPointerofObject(objStackRootIndex, hiddenRootsObj);
  return (stackOrNil == nilObj) || (isValidObjStackPagemyIndexfirstPage(
                                       stackOrNil, objStackRootIndex, 1));
}