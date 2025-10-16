/* Extracted from interp.c:45250 (function runLeakCheckerForFreeSpaceignoring). */

static NoDbgRegParms void
runLeakCheckerForFreeSpaceignoring(sqInt gcModes, sqInt anOopOrNil)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (((gcModes & GCCheckFreeSpace) != 0)) {
		reverseDisplayFromto(16, 19);
		clearLeakMapAndMapAccessibleFreeSpace();
		GIV(freeSpaceCheckOopToIgnore) = anOopOrNil;
		asserta(checkHeapFreeSpaceIntegrity());
		GIV(freeSpaceCheckOopToIgnore) = null;
	}
}