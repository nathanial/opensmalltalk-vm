/* Extracted from interp.c:45263 (function runLeakCheckerFor). */

static NoDbgRegParms void
runLeakCheckerFor(sqInt gcModes)
{
	/* begin inLineRunLeakCheckerFor:excludeUnmarkedObjs:classIndicesShouldBeValid: */
	if (((gcModes & checkForLeaks) != 0)) {
		if (((gcModes & GCModeFull) != 0)) {
			reverseDisplayFromto(0, 7);
		}
		else {
			reverseDisplayFromto(8, 15);
		}
		clearLeakMapAndMapAccessibleObjects();
		asserta(checkHeapIntegrityclassIndicesShouldBeValid(0, 1));
		asserta((checkInterpreterIntegrity()) == 0);
		asserta(checkStackIntegrity());
		asserta(checkCodeIntegrity(gcModes));
		if (((gcModes & GCCheckFreeSpace) != 0)) {
			clearLeakMapAndMapAccessibleFreeSpace();
			asserta(checkHeapFreeSpaceIntegrity());
		}
	}
	return;
}