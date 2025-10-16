/* Extracted from BitBltPlugin.c:3522 (function moduleUnloaded). */
/* BitBltSimulation>>#moduleUnloaded: */
/*	Subract the pixels in the source and destination, color by color,
        and return the sum of the absolute value of all the differences.
        For non-rgb, XOR the two and return the number of differing pixels.
        Note that the region is not clipped to bit boundaries, but only to the
        nearest (enclosing) word. This is because copyLoop does not do
        pre-merge masking. For accurate results, you must subtract the
        values obtained from the left and right fringes. */

EXPORT(sqInt)
moduleUnloaded(char *aModuleName) {
  if (!(strcmp(aModuleName, "SurfacePlugin"))) {
    querySurfaceFn = 0;
    lockSurfaceFn = 0;
    unlockSurfaceFn = 0;
  }
  return 0;
}
