/* Extracted from interp.c:27555 (function numFreeLists). */

/*	Answer the number of free lists. We use freeListsMask, a bitmap, to
   avoid reading empty list heads. This should fit in a machine word to end up
   in a register during free chunk allocation. */

/* Spur64BitMemoryManager>>#numFreeLists */

static sqInt numFreeLists(void) { return 64; }