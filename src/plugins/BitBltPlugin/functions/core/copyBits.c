/* Extracted from BitBltPlugin.c:1388 (function copyBits). */
/* BitBltSimulation>>#copyBits */
/*	Perform the actual copyBits operation using the fast path specialised
        code; fail some cases by falling back to normal code.
        Assume: Surfaces have been locked and clipping was performed.
 */

EXPORT(sqInt)
copyBits(void) {
  clipRange();
  if ((bbW <= 0) || (bbH <= 0)) {
    affectedL = (affectedR = (affectedT = (affectedB = 0)));
    return null;
  }

  /* zero width or height; noop
     Lock the surfaces */
  if (!(lockSurfaces())) {
    return primitiveFail();
  }
  copyBitsLockedAndClipped();

  /* you really, really mustn't call this unless you have the rest of the code
   * to link to */
  unlockSurfaces();
  return 0;
}
