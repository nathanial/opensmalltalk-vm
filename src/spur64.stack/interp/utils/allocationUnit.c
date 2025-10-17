/* Extracted from interp.c:31062 (function allocationUnit). */

/*	All objects are a multiple of 8 bytes in length */

/* SpurMemoryManager>>#allocationUnit */

static sqInt allocationUnit(void) { return 8; }