/* Extracted from interp.c:45288 (function
 * runLeakCheckerForexcludeUnmarkedObjsclassIndicesShouldBeValid). */

/* SpurMemoryManager>>#runLeakCheckerFor:excludeUnmarkedObjs:classIndicesShouldBeValid:
 */

static NoDbgRegParms void
runLeakCheckerForexcludeUnmarkedObjsclassIndicesShouldBeValid(
    sqInt gcModes, sqInt excludeUnmarkedObjs, sqInt classIndicesShouldBeValid) {
  /* begin
   * inLineRunLeakCheckerFor:excludeUnmarkedObjs:classIndicesShouldBeValid: */
  if (((gcModes & checkForLeaks) != 0)) {
    if (((gcModes & GCModeFull) != 0)) {
      reverseDisplayFromto(0, 7);
    } else {
      reverseDisplayFromto(8, 15);
    }
    clearLeakMapAndMapAccessibleObjects();
    asserta(checkHeapIntegrityclassIndicesShouldBeValid(
        excludeUnmarkedObjs, classIndicesShouldBeValid));
    asserta((checkInterpreterIntegrity()) == 0);
    asserta(checkStackIntegrity());
    asserta(checkCodeIntegrity(gcModes));
    if (((gcModes & GCCheckFreeSpace) != 0)) {
      clearLeakMapAndMapAccessibleFreeSpace();
      asserta(checkHeapFreeSpaceIntegrity());
    }
  }
}