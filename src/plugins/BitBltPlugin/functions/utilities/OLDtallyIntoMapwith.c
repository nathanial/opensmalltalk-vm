/* Extracted from BitBltPlugin.c:3585 (function OLDtallyIntoMapwith). */
/* BitBltSimulation>>#OLDtallyIntoMap:with: */
/*	Add word1 to word2 as nParts partitions of nBits each.
        This is useful for packed pixels, or packed colors */
/*	Use unsigned int everywhere because it has a well known arithmetic model
        without undefined behavior w.r.t. overflow and shifts
 */

static sqInt OLDtallyIntoMapwith(sqInt sourceWord, sqInt destinationWord) {
  sqInt d;
  sqInt destPix;
  sqInt i;
  sqInt mapIndex;
  sqInt mask;
  sqInt pixMask;
  sqInt shiftWord;
  sqInt srcPix;

  if (!((cmFlags & (ColorMapPresent | ColorMapIndexedPart)) ==
        (ColorMapPresent | ColorMapIndexedPart))) {
    return destinationWord;
  }
  if (destDepth < 16) {
    pixMask = (maskTable[destDepth]) & cmMask;
    shiftWord = destinationWord;
    for (i = 1; i <= destPPW; i += 1) {
      mapIndex = shiftWord & pixMask;

      /* tallyMapAt:put: */
      cmLookupTable[mapIndex & cmMask] =
          ((cmLookupTable[mapIndex & cmMask]) + 1);
      shiftWord = ((usqInt)(shiftWord)) >> destDepth;
    }
    return destinationWord;
  }

  /* loop through all packed pixels. */
  if (destDepth == 16) {
    /* begin rgbMap:from:to: */
    if (((d = cmBitsPerColor - 5)) > 0) {
      /* Transfer mask */
      mask = 0x1F;
      srcPix = (((usqInt)((destinationWord & 0xFFFF)) << d));
      mask = ((sqInt)((usqInt)(mask) << d));
      destPix = srcPix & mask;
      mask = ((sqInt)((usqInt)(mask) << cmBitsPerColor));
      srcPix = ((sqInt)((usqInt)(srcPix) << d));
      mapIndex = (destPix + (srcPix & mask)) +
                 (((((usqInt)(srcPix) << d))) &
                  ((((usqInt)(mask) << cmBitsPerColor))));
    } else {
      if (!d) {
        mapIndex = (destinationWord & 0xFFFF) & 0x7FFF;
        goto l1;

        /* Sometimes called with 16 bits, though pixel is 15,
           but we must never return more than 15. */

        /* Sometimes called with 32 bits, though pixel is 24,
           but we must never return more than 24. */
        mapIndex = destinationWord & 0xFFFF;
        goto l1;
      }
      if (!(destinationWord & 0xFFFF)) {
        mapIndex = destinationWord & 0xFFFF;
        goto l1;
      }
      d = 5 - cmBitsPerColor;

      /* Transfer mask */
      mask = (1U << cmBitsPerColor) - 1;
      srcPix = ((usqInt)((destinationWord & 0xFFFF))) >> d;
      destPix = srcPix & mask;
      mask = ((sqInt)((usqInt)(mask) << cmBitsPerColor));
      srcPix = ((usqInt)(srcPix)) >> d;
      destPix =
          (destPix + (srcPix & mask)) +
          ((((usqInt)(srcPix)) >> d) & ((((usqInt)(mask) << cmBitsPerColor))));
      if (!destPix) {
        mapIndex = 1;
        goto l1;
      }
      mapIndex = destPix;
    }
    /* end rgbMap:from:to: */
  l1:

    /* tallyMapAt:put: */
    cmLookupTable[mapIndex & cmMask] = ((cmLookupTable[mapIndex & cmMask]) + 1);

    /* ... and then left half */

    /* begin rgbMap:from:to: */
    if (((d = cmBitsPerColor - 5)) > 0) {
      /* Transfer mask */
      mask = 0x1F;
      srcPix = ((sqInt)((usqInt)((((usqInt)(destinationWord)) >> 16)) << d));
      mask = ((sqInt)((usqInt)(mask) << d));
      destPix = srcPix & mask;
      mask = ((sqInt)((usqInt)(mask) << cmBitsPerColor));
      srcPix = ((sqInt)((usqInt)(srcPix) << d));
      mapIndex = (destPix + (srcPix & mask)) +
                 (((((usqInt)(srcPix) << d))) &
                  ((((usqInt)(mask) << cmBitsPerColor))));
    } else {
      if (!d) {
        mapIndex = (((usqInt)(destinationWord)) >> 16) & 0x7FFF;
        goto l2;

        /* Sometimes called with 16 bits, though pixel is 15,
           but we must never return more than 15. */

        /* Sometimes called with 32 bits, though pixel is 24,
           but we must never return more than 24. */
        mapIndex = ((usqInt)(destinationWord)) >> 16;
        goto l2;
      }
      if (!(((usqInt)(destinationWord)) >> 16)) {
        mapIndex = ((usqInt)(destinationWord)) >> 16;
        goto l2;
      }
      d = 5 - cmBitsPerColor;

      /* Transfer mask */
      mask = (1U << cmBitsPerColor) - 1;
      srcPix = ((usqInt)((((usqInt)(destinationWord)) >> 16))) >> d;
      destPix = srcPix & mask;
      mask = ((sqInt)((usqInt)(mask) << cmBitsPerColor));
      srcPix = ((usqInt)(srcPix)) >> d;
      destPix =
          (destPix + (srcPix & mask)) +
          ((((usqInt)(srcPix)) >> d) & ((((usqInt)(mask) << cmBitsPerColor))));
      if (!destPix) {
        mapIndex = 1;
        goto l2;
      }
      mapIndex = destPix;
    }
    /* end rgbMap:from:to: */
  l2:

    /* tallyMapAt:put: */
    cmLookupTable[mapIndex & cmMask] = ((cmLookupTable[mapIndex & cmMask]) + 1);
  } else {
    /* begin rgbMap:from:to: */
    if (((d = cmBitsPerColor - 8)) > 0) {
      /* Transfer mask */
      mask = 0xFF;
      srcPix = ((sqInt)((usqInt)(destinationWord) << d));
      mask = ((sqInt)((usqInt)(mask) << d));
      destPix = srcPix & mask;
      mask = ((sqInt)((usqInt)(mask) << cmBitsPerColor));
      srcPix = ((sqInt)((usqInt)(srcPix) << d));
      mapIndex = (destPix + (srcPix & mask)) +
                 (((((usqInt)(srcPix) << d))) &
                  ((((usqInt)(mask) << cmBitsPerColor))));
    } else {
      if (!d) {

        /* Sometimes called with 16 bits, though pixel is 15,
           but we must never return more than 15. */
        mapIndex = destinationWord & 0xFFFFFF;
        goto l3;

        /* Sometimes called with 32 bits, though pixel is 24,
           but we must never return more than 24. */
        mapIndex = destinationWord;
        goto l3;
      }
      if (!destinationWord) {
        mapIndex = destinationWord;
        goto l3;
      }
      d = 8 - cmBitsPerColor;

      /* Transfer mask */
      mask = (1U << cmBitsPerColor) - 1;
      srcPix = ((usqInt)(destinationWord)) >> d;
      destPix = srcPix & mask;
      mask = ((sqInt)((usqInt)(mask) << cmBitsPerColor));
      srcPix = ((usqInt)(srcPix)) >> d;
      destPix =
          (destPix + (srcPix & mask)) +
          ((((usqInt)(srcPix)) >> d) & ((((usqInt)(mask) << cmBitsPerColor))));
      if (!destPix) {
        mapIndex = 1;
        goto l3;
      }
      mapIndex = destPix;
    }
    /* end rgbMap:from:to: */
  l3:

    /* tallyMapAt:put: */
    cmLookupTable[mapIndex & cmMask] = ((cmLookupTable[mapIndex & cmMask]) + 1);
  }

  /* Two pixels  Tally the right half...
     Just one pixel. */
  return destinationWord;
}
