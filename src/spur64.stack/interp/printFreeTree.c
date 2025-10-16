/* Extracted from interp.c:43344 (function printFreeTree). */

void
printFreeTree(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	printFreeTreeChunk(GIV(freeLists)[0]);
}