/* Extracted from interp.c:45090 (function removeGCRoot). */

/*	Remove the given variable location to the extra roots table. */

	/* SpurMemoryManager>>#removeGCRoot: */

sqInt
removeGCRoot(sqInt *varLoc)
{
    int i;

	for (i = 1; i <= extraRootCount; i++) {
		if (varLoc == (extraRoots[i])) {
			extraRoots[i] = (extraRoots[extraRootCount]);
			extraRootCount -= 1;
			return 1;
		}
	}
	return 0;
}