/* Extracted from BitBltPlugin.c:4777 (function primitiveWarpBits). */

EXPORT(sqInt)
primitiveWarpBits(void) {
  sqInt endBits;
  sqInt ns;
  sqInt pixPerM1;
  sqInt rcvr;
  sqInt startBits;

  rcvr = stackValue(methodArgumentCount());
  if (!(loadBitBltFromwarping(rcvr, 1))) {
    return primitiveFail();
  }

  /* begin warpBits */
  ns = noSource;
  noSource = 1;
  clipRange();
  noSource = ns;
  if (noSource || ((bbW <= 0) || (bbH <= 0))) {
    affectedL = (affectedR = (affectedT = (affectedB = 0)));
    goto l1;
  }

  /* zero width or height; noop */
  if (!(lockSurfaces())) {
    primitiveFail();
    goto l1;
  }

  /* begin destMaskAndPointerInit */
  /* A mask, assuming power of two */
  /* how many pixels in first word */
  pixPerM1 = destPPW - 1;
  startBits = destPPW - (dx & pixPerM1);

  /* how many pixels in last word */
  endBits = (((dx + bbW) - 1) & pixPerM1) + 1;
  if (destMSB) {
    mask1 = ((usqInt)(AllOnes)) >> (32 - (startBits * destDepth));
    mask2 = ((((usqInt)(AllOnes) << (32 - (endBits * destDepth))))) & AllOnes;
  } else {
    mask1 = ((((usqInt)(AllOnes) << (32 - (startBits * destDepth))))) & AllOnes;
    mask2 = ((usqInt)(AllOnes)) >> (32 - (endBits * destDepth));
  }

  /* determine number of words stored per line; merge masks if only 1 */
  if (bbW <= startBits) {
    mask1 = mask1 & mask2;
    mask2 = 0;
    nWords = 1;
  } else {
    nWords = (((bbW - startBits) + pixPerM1) / destPPW) + 1;
  }

  /* defaults for no overlap with source */
  /* calculate byte addr and delta, based on first word of data */
  /* Note pitch is bytes and nWords is longs, not bytes */
  hDir = (vDir = 1);
  destIndex = (destBits + (dy * destPitch)) + ((dx / destPPW) * 4);

  /* byte addr delta */
  destDelta = (destPitch * vDir) - (4 * (nWords * hDir));
  warpLoop();
  if (hDir > 0) {
    affectedL = dx;
    affectedR = dx + bbW;
  } else {
    affectedL = (dx - bbW) + 1;
    affectedR = dx + 1;
  }
  if (vDir > 0) {
    affectedT = dy;
    affectedB = dy + bbH;
  } else {
    affectedT = (dy - bbH) + 1;
    affectedB = dy + 1;
  }
  unlockSurfaces();
  /* end warpBits */
l1:
  if (failed()) {
    return null;
  }

  /* begin showDisplayBits */
  /* begin ensureDestAndSourceFormsAreValid */
  if (numGCsOnInvocation != (statNumGCs())) {
    reloadDestAndSourceForms();
  }
  showDisplayBitsLeftTopRightBottom(destForm, affectedL, affectedT, affectedR,
                                    affectedB);
  if (failed()) {
    return null;
  }
  methodReturnReceiver();
  return 0;
}