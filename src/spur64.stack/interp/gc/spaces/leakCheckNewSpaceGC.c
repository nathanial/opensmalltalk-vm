/* Extracted from interp.c:38114 (function leakCheckNewSpaceGC). */

/* SpurMemoryManager>>#leakCheckNewSpaceGC */

int leakCheckNewSpaceGC(void) {
  return ((checkForLeaks & GCModeNewSpace) != 0);
}