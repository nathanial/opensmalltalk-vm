/* Extracted from interp.c:45715 (function storeCheckBoundary). */

/*	A renaming for the Cogit, which can't make sense of GIV(oldSpaceStart) */

	/* SpurMemoryManager>>#storeCheckBoundary */

usqInt
storeCheckBoundary(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(oldSpaceStart);
}