/* Extracted from interp.c:41626 (function objectsAccessibleFromRoots). */

static NoDbgRegParms sqInt
objectsAccessibleFromRoots(sqInt arrayOfRootsArg)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arrayOfObjects;
    sqInt arrayOfRoots;

	runLeakCheckerFor(GCModeImageSegment);

	/* First scavenge to collect any new space garbage that refers to the graph. */

	/* begin scavengingGC */
	scavengingGCTenuringIf(TenureByAge);
	arrayOfRoots = /* updatePostScavenge: */
			((!((longAt((void *)(arrayOfRootsArg))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
				? followForwarded(arrayOfRootsArg)
				: arrayOfRootsArg);

	/* Now compute the transitive closure, collecting the sequence of objects to be stored in the arrayOfObjects array.
	   Included in arrayOfObjects are the arrayOfRoots and all its contents.  All objects have been unmarked. */
	arrayOfObjects = objectsReachableFromRoots(arrayOfRoots);
	if (!arrayOfObjects) {
		return (((usqInt)PrimErrNoMemory << 3) | 1);
	}

	/* If objectsReachableFromRoots: answers an integer there is not enough continuous free space in which to allocate the
	   reachable objects.  If there is sufficient free space then answer an error code to prompt a compacting GC and a retry. */
	if ((((arrayOfObjects) & 7) == 1)) {
		if ((GIV(totalFreeOldSpace) - 8 /* allocationUnit */) >= ((arrayOfObjects >> 3))) {
			return (((usqInt)PrimErrNeedCompaction << 3) | 1);
		}
		return (((usqInt)PrimErrNoMemory << 3) | 1);
	}
	assert(allObjectsUnmarked());
	assert(!((forwardersIn(arrayOfObjects))));
	return arrayOfObjects;
}