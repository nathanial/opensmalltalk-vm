/* Extracted from BitBltPlugin.c:1388 (function copyBits). */

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
#if ENABLE_FAST_BLT
  copyBitsFastPathSpecialised();
#else
  copyBitsLockedAndClipped();
#endif

  /* you really, really mustn't call this unless you have the rest of the code
   * to link to */
  unlockSurfaces();
  return 0;
}