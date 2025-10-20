/* Extracted from BitBltPlugin.c:3363 (function loadSurfacePlugin). */

static sqInt loadSurfacePlugin(void) {
  querySurfaceFn = ioLoadFunctionFrom("ioGetSurfaceFormat", "SurfacePlugin");
  lockSurfaceFn = ioLoadFunctionFrom("ioLockSurface", "SurfacePlugin");
  unlockSurfaceFn = ioLoadFunctionFrom("ioUnlockSurface", "SurfacePlugin");
  return (querySurfaceFn != 0) &&
         ((lockSurfaceFn != 0) && (unlockSurfaceFn != 0));
}