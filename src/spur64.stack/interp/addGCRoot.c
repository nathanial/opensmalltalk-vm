/* Extracted from interp.c:29715 (function addGCRoot). */

/*	Add the given variable location to the extra roots table. */

/* SpurMemoryManager>>#addGCRoot: */

sqInt addGCRoot(sqInt *varLoc) {
  if (extraRootCount >= ExtraRootsSize) {
    return 0;
  }
  extraRoots[(extraRootCount += 1)] = varLoc;
  return 1;
}