/* Extracted from BitBltPlugin.c:4250 (function primitiveDisplayString). */
/* BitBltSimulation>>#primitiveDisplayString */
/*	Invoke the line drawing primitive. */

EXPORT(sqInt)
primitiveDisplayString(void) {
  sqInt ascii;
  sqInt bbObj;
  sqInt charIndex;
  sqInt endBits;
  sqInt glyphIndex;
  sqInt glyphMap;
  sqInt kernDelta;
  sqInt left;
  sqInt maxGlyph;
  sqInt pixPerM1;
  sqInt quickBlt;
  char *sourcePtr;
  sqInt sourceString;
  sqInt startBits;
  sqInt startIndex;
  sqInt stopIndex;
  sqInt xTable;

  if (!((methodArgumentCount()) == 6)) {
    return primitiveFail();
  }
  kernDelta = stackIntegerValue(0);
  xTable = stackValue(1);
  glyphMap = stackValue(2);
  stopIndex = stackIntegerValue(3);
  startIndex = stackIntegerValue(4);
  sourceString = stackValue(5);
  bbObj = stackObjectValue(6);
  if (failed()) {
    return null;
  }
  if (!(loadBitBltFromwarping(bbObj, 0))) {
    return primitiveFailFor(PrimErrBadReceiver);
  }
  if (!((isArray(xTable)) &&
        ((isArray(glyphMap)) &&
         (((slotSizeOf(glyphMap)) == 0x100) &&
          ((isBytes(sourceString)) &&
           ((startIndex > 0) &&
            ((stopIndex >= 0) &&
             ((stopIndex <= (byteSizeOf(sourceString))) &&
              ((combinationRule != 30) && (combinationRule != 0x1F)))))))))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if (!stopIndex) {
    return pop(6);
  }
  maxGlyph = (slotSizeOf(xTable)) - 2;

  /* See if we can go directly into copyLoopPixMap (usually we can) */

  /* no OS surfaces please */
  /* no point using slower version */
  quickBlt =
      (destBits != 0) &&
      ((sourceBits != 0) &&
       ((noSource == 0) && ((sourceForm != destForm) &&
                            ((cmFlags != 0) || ((sourceMSB != destMSB) ||
                                                (sourceDepth != destDepth))))));
  if (quickBlt) {
    endOfSource = sourceBits + (sourcePitch * sourceHeight);
    endOfDestination = destBits + (destPitch * destHeight);
  } else {
    if (!(lockSurfaces())) {
      return primitiveFail();
    }
  }
  left = destX;
  sourcePtr = firstIndexableField(sourceString);
  for (charIndex = startIndex; charIndex <= stopIndex; charIndex += 1) {
    ascii = byteAt((void *)((sourcePtr + charIndex) - 1));
    glyphIndex = fetchIntegerofObject(ascii, glyphMap);
    if ((glyphIndex < 0) || (glyphIndex > maxGlyph)) {
      return primitiveFailFor(PrimErrBadIndex);
    }
    sourceX = fetchIntegerofObject(glyphIndex, xTable);
    width = (fetchIntegerofObject(glyphIndex + 1, xTable)) - sourceX;
    if (failed()) {
      return null;
    }
    clipRange();
    if ((bbW > 0) && (bbH > 0)) {
      if (quickBlt) {
        /* begin destMaskAndPointerInit */
        /* A mask, assuming power of two */
        /* how many pixels in first word */
        pixPerM1 = destPPW - 1;
        startBits = destPPW - (dx & pixPerM1);

        /* how many pixels in last word */
        endBits = (((dx + bbW) - 1) & pixPerM1) + 1;
        if (destMSB) {
          mask1 = ((usqInt)(AllOnes)) >> (32 - (startBits * destDepth));
          mask2 =
              ((((usqInt)(AllOnes) << (32 - (endBits * destDepth))))) & AllOnes;
        } else {
          mask1 = ((((usqInt)(AllOnes) << (32 - (startBits * destDepth))))) &
                  AllOnes;
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
        copyLoopPixMap();

        /* both, hDir and vDir are known to be > 0 */
        affectedL = dx;
        affectedR = dx + bbW;
        affectedT = dy;
        affectedB = dy + bbH;
      } else {
        copyBitsLockedAndClipped();
      }
    }
    if (failed()) {
      return null;
    }
    destX = (destX + width) + kernDelta;
  }
  affectedL = left;
  if (!quickBlt) {
    unlockSurfaces();
  }

  /* begin showDisplayBits */
  /* begin ensureDestAndSourceFormsAreValid */
  if (numGCsOnInvocation != (statNumGCs())) {
    reloadDestAndSourceForms();
  }
  showDisplayBitsLeftTopRightBottom(destForm, affectedL, affectedT, affectedR,
                                    affectedB);

  /* store destX back */
  storeIntegerofObjectwithValue(BBDestXIndex, bbObj, destX);
  pop(6);
  return 0;
}
