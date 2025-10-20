/* Extracted from BitBltPlugin.c:5963 (function tallyIntoMapwith). */
/* BitBltSimulation>>#tallyIntoMap:with: */
/*	Unlock the bits of any OS surfaces. */
/*	See the comment in lockSurfaces. Similar rules apply. That is, the area
        provided in ioUnlockSurface
        can be used to determine the dirty region after drawing. If a source is
        unlocked, then the area will
        be (0,0,0,0) to indicate that no portion is dirty. Note that if a GC
        happens during unlockSourceFn
        (if it is effectively a callback) no matter. No bits are touched after
        unlock.  */

static unsigned int tallyIntoMapwith(unsigned int sourceWord,
                                     unsigned int destinationWord) {
  sqInt d;
  sqInt destPix;
  sqInt destPixSqInt;
  unsigned int destShifted;
  sqInt dSqInt;
  sqInt i;
  sqInt mapIndex;
  sqInt mask;
  unsigned int maskShifted;
  sqInt maskSqInt;
  unsigned int pixMask;
  unsigned int pixVal;
  sqInt srcPix;
  sqInt srcPixSqInt;

  if (!((cmFlags & (ColorMapPresent | ColorMapIndexedPart)) ==
        (ColorMapPresent | ColorMapIndexedPart))) {
    return destinationWord;
  }
  pixMask = maskTable[destDepth];
  destShifted = destinationWord;
  maskShifted = destMask;
  for (i = 1; i <= destPPW; i += 1) {
    if (maskShifted & pixMask) {
      pixVal = destShifted & pixMask;
      if (destDepth < 16) {
        mapIndex = pixVal;
      } else {
        if (destDepth == 16) {
          /* begin rgbMap:from:to: */
          if (((d = cmBitsPerColor - 5)) > 0) {
            /* Transfer mask */
            mask = 0x1F;
            srcPix = (((usqInt)(pixVal) << d));
            mask = ((sqInt)((usqInt)(mask) << d));
            destPix = srcPix & mask;
            mask = ((sqInt)((usqInt)(mask) << cmBitsPerColor));
            srcPix = ((sqInt)((usqInt)(srcPix) << d));
            mapIndex = (destPix + (srcPix & mask)) +
                       (((((usqInt)(srcPix) << d))) &
                        ((((usqInt)(mask) << cmBitsPerColor))));
          } else {
            if (!d) {
              mapIndex = pixVal & 0x7FFF;
              goto l1;

              /* Sometimes called with 16 bits, though pixel is 15,
                 but we must never return more than 15. */

              /* Sometimes called with 32 bits, though pixel is 24,
                 but we must never return more than 24. */
              mapIndex = pixVal;
              goto l1;
            }
            if (!pixVal) {
              mapIndex = pixVal;
              goto l1;
            }
            d = 5 - cmBitsPerColor;

            /* Transfer mask */
            mask = (1U << cmBitsPerColor) - 1;
            srcPix = ((usqInt)(pixVal)) >> d;
            destPix = srcPix & mask;
            mask = ((sqInt)((usqInt)(mask) << cmBitsPerColor));
            srcPix = ((usqInt)(srcPix)) >> d;
            destPix = (destPix + (srcPix & mask)) +
                      ((((usqInt)(srcPix)) >> d) &
                       ((((usqInt)(mask) << cmBitsPerColor))));
            if (!destPix) {
              mapIndex = 1;
              goto l1;
            }
            mapIndex = destPix;
          }
          /* end rgbMap:from:to: */
        l1:;
        } else {
          /* begin rgbMap:from:to: */
          if (((dSqInt = cmBitsPerColor - 8)) > 0) {
            /* Transfer mask */
            maskSqInt = 0xFF;
            srcPixSqInt = (((usqInt)(pixVal) << dSqInt));
            maskSqInt = ((sqInt)((usqInt)(maskSqInt) << dSqInt));
            destPixSqInt = srcPixSqInt & maskSqInt;
            maskSqInt = ((sqInt)((usqInt)(maskSqInt) << cmBitsPerColor));
            srcPixSqInt = ((sqInt)((usqInt)(srcPixSqInt) << dSqInt));
            mapIndex = (destPixSqInt + (srcPixSqInt & maskSqInt)) +
                       (((((usqInt)(srcPixSqInt) << dSqInt))) &
                        ((((usqInt)(maskSqInt) << cmBitsPerColor))));
          } else {
            if (!dSqInt) {

              /* Sometimes called with 16 bits, though pixel is 15,
                 but we must never return more than 15. */
              mapIndex = pixVal & 0xFFFFFF;
              goto l2;

              /* Sometimes called with 32 bits, though pixel is 24,
                 but we must never return more than 24. */
              mapIndex = pixVal;
              goto l2;
            }
            if (!pixVal) {
              mapIndex = pixVal;
              goto l2;
            }
            dSqInt = 8 - cmBitsPerColor;

            /* Transfer mask */
            maskSqInt = (1U << cmBitsPerColor) - 1;
            srcPixSqInt = ((usqInt)(pixVal)) >> dSqInt;
            destPixSqInt = srcPixSqInt & maskSqInt;
            maskSqInt = ((sqInt)((usqInt)(maskSqInt) << cmBitsPerColor));
            srcPixSqInt = ((usqInt)(srcPixSqInt)) >> dSqInt;
            destPixSqInt = (destPixSqInt + (srcPixSqInt & maskSqInt)) +
                           ((((usqInt)(srcPixSqInt)) >> dSqInt) &
                            ((((usqInt)(maskSqInt) << cmBitsPerColor))));
            if (!destPixSqInt) {
              mapIndex = 1;
              goto l2;
            }
            mapIndex = destPixSqInt;
          }
          /* end rgbMap:from:to: */
        l2:;
        }
      }

      /* tallyMapAt:put: */
      cmLookupTable[mapIndex & cmMask] =
          ((cmLookupTable[mapIndex & cmMask]) + 1);
    }

    /* Only tally pixels within the destination rectangle */
    maskShifted = ((usqInt)(maskShifted)) >> destDepth;
    destShifted = ((usqInt)(destShifted)) >> destDepth;
  }
  return destinationWord;
}
