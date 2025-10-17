/* Extracted from interp.c:45090 (function removeGCRoot). */

/*	Remove the given variable location to the extra roots table. */

	/* SpurMemoryManager>>#removeGCRoot: */

sqInt
removeGCRoot(sqInt *varLoc)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int i;

	for (i = 1; i <= GIV(extraRootCount); i++) {
		if (varLoc == (GIV(extraRoots)[i])) {
			GIV(extraRoots)[i] = (GIV(extraRoots)[GIV(extraRootCount)]);
			GIV(extraRootCount) -= 1;
			return 1;
		}
	}
	return 0;
}