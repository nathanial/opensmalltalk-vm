/* Extracted from BitBltPlugin.c:3522 (function moduleUnloaded). */

EXPORT(sqInt)
moduleUnloaded(char *aModuleName) {
  if (!(strcmp(aModuleName, "SurfacePlugin"))) {
    querySurfaceFn = 0;
    lockSurfaceFn = 0;
    unlockSurfaceFn = 0;
  }
  return 0;
}