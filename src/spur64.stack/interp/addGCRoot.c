/* Extracted from interp.c:29715 (function addGCRoot). */

/*	Add the given variable location to the extra roots table. */

	/* SpurMemoryManager>>#addGCRoot: */

sqInt
addGCRoot(sqInt *varLoc)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (GIV(extraRootCount) >= ExtraRootsSize) {
		return 0;
	}
	GIV(extraRoots)[(GIV(extraRootCount) += 1)] = varLoc;
	return 1;
}