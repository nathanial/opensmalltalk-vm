/* Extracted from interp.c:45250 (function runLeakCheckerForFreeSpaceignoring).
 */

/*	Check free space integrity by setting bits in the map corresponding to
   all free space objects and checking tat no pointer field refers to a free
   object. anOopOrNil is provided to filter-out the as-yet-to-be initialized
   object in primitiveShallowCopy/primitiveClone.
 */

/* SpurMemoryManager>>#runLeakCheckerForFreeSpace:ignoring: */

static void runLeakCheckerForFreeSpaceignoring(sqInt gcModes,
                                               sqInt anOopOrNil) {
  if (((gcModes & GCCheckFreeSpace) != 0)) {
    reverseDisplayFromto(16, 19);
    clearLeakMapAndMapAccessibleFreeSpace();
    freeSpaceCheckOopToIgnore = anOopOrNil;
    asserta(checkHeapFreeSpaceIntegrity());
    freeSpaceCheckOopToIgnore = null;
  }
}