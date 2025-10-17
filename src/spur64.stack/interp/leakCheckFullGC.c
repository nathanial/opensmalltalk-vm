/* Extracted from interp.c:38107 (function leakCheckFullGC). */

/* SpurMemoryManager>>#leakCheckFullGC */

int leakCheckFullGC(void) { return ((checkForLeaks & GCModeFull) != 0); }