/* Extracted from BitBltPlugin.c:6117 (function unlockSurfaces). */

static sqInt unlockSurfaces(void) {
  sqInt destHandle;
  sqInt destLocked;
  sqInt sourceHandle;

  if (!hasSurfaceLock) {
    return 0;
  }
  if (!unlockSurfaceFn) {
    if (!(loadSurfacePlugin())) {
      return 0;
    }
  }

  /* begin ensureDestAndSourceFormsAreValid */
  if (numGCsOnInvocation != (statNumGCs())) {
    reloadDestAndSourceForms();
  }
  destLocked = 0;
  destHandle = fetchPointerofObject(FormBitsIndex, destForm);
  if (isIntegerObject(destHandle)) {
    unlockSurfaceFn(integerValueOf(destHandle), affectedL, affectedT,
                    affectedR - affectedL, affectedB - affectedT);
    destBits = (destPitch = 0);
    destLocked = 1;
  }

  /* The destBits are always assumed to be dirty */
  if (!noSource) {
    /* begin ensureDestAndSourceFormsAreValid */
    if (numGCsOnInvocation != (statNumGCs())) {
      reloadDestAndSourceForms();
    }
    sourceHandle = fetchPointerofObject(FormBitsIndex, sourceForm);
    if (isIntegerObject(sourceHandle)) {
      if (!(destLocked && (sourceHandle == destHandle))) {
        unlockSurfaceFn(integerValueOf(sourceHandle), 0, 0, 0, 0);
      }
      sourceBits = (sourcePitch = 0);
    }
  }
  hasSurfaceLock = 0;
  return 0;
}