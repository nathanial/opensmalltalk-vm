/* Extracted from BitBltPlugin.c:1055 (function alphaSourceBlendBits8). */

static sqInt alphaSourceBlendBits8(void) {
  sqInt adjust;
  int deltaX;
  int deltaY;
  unsigned int destWord;
  sqInt dstIndex;
  unsigned int dstMask;
  sqInt dstValue;
  sqInt dstY;
  sqInt mapperFlags;
  unsigned int *mappingTable;
  sqInt pv;
  unsigned int sourceWord;
  unsigned int srcAlpha;
  sqInt srcIndex;
  sqInt srcShift;
  sqInt srcY;
  sqInt val;

  mappingTable = default8To32Table();
  mapperFlags = cmFlags & ((unsigned int)~ColorMapNewStyle);

  /* So we can pre-decrement */
  deltaY = bbH + 1;
  srcY = sy;
  dstY = dy;
  mask1 = (dx & 3) * 8;
  if (destMSB) {
    mask1 = 24 - mask1;
  }
  mask2 = AllOnes ^ (0xFFU << mask1);
  if (dx & 1) {
    adjust = 0x1F1F1F1F;
  } else {
    adjust = 0;
  }
  if (!(dy & 1)) {
    adjust = adjust ^ 0x1F1F1F1F;
  }

  /* This is the outer loop */
  while (((deltaY -= 1)) != 0) {
    adjust = adjust ^ 0x1F1F1F1F;
    srcIndex = (sourceBits + (srcY * sourcePitch)) + (sx * 4);
    dstIndex = (destBits + (dstY * destPitch)) + ((dx / 4) * 4);

    /* So we can pre-decrement */
    deltaX = bbW + 1;
    srcShift = mask1;
    dstMask = mask2;

    /* This is the inner loop */
    while (((deltaX -= 1)) != 0) {
      sourceWord = (((/* begin srcLongAt: */
                      assert((((usqInt)srcIndex)) < endOfSource),
                      long32At((void *)(srcIndex)))) &
                    ((unsigned int)~adjust)) +
                   adjust;
      srcAlpha = ((usqInt)(sourceWord)) >> 24;
      if (srcAlpha > 0x1F) {
        if (srcAlpha < 224) {
          /* begin dstLongAt: */
          assert((((usqInt)dstIndex)) < endOfDestination);
          destWord = long32At((void *)(dstIndex));
          destWord = destWord & ((unsigned int)~dstMask);
          destWord = ((usqInt)(destWord)) >> srcShift;
          destWord = mappingTable[destWord];
          sourceWord = alphaBlendScaledwith(sourceWord, destWord);
        }

        /* Everything above 224 is opaque */

        /* begin mapPixel:flags: */
        pv = sourceWord;
        if (mapperFlags & ColorMapPresent) {
          if (mapperFlags & ColorMapFixedPart) {
            /* begin rgbMapPixel:flags: */
            val = (((((int)(cmShiftTable[0]))) < 0)
                       ? (((usqInt)((sourceWord & (cmMaskTable[0])))) >>
                          (-(((int)(cmShiftTable[0])))))
                       : ((((usqInt)((sourceWord & (cmMaskTable[0])))
                            << (((int)(cmShiftTable[0])))))));
            val = val | ((((((int)(cmShiftTable[1]))) < 0)
                              ? (((usqInt)((sourceWord & (cmMaskTable[1])))) >>
                                 (-(((int)(cmShiftTable[1])))))
                              : ((((usqInt)((sourceWord & (cmMaskTable[1])))
                                   << (((int)(cmShiftTable[1]))))))));
            val = val | ((((((int)(cmShiftTable[2]))) < 0)
                              ? (((usqInt)((sourceWord & (cmMaskTable[2])))) >>
                                 (-(((int)(cmShiftTable[2])))))
                              : ((((usqInt)((sourceWord & (cmMaskTable[2])))
                                   << (((int)(cmShiftTable[2]))))))));
            pv = val | ((((((int)(cmShiftTable[3]))) < 0)
                             ? (((usqInt)((sourceWord & (cmMaskTable[3])))) >>
                                (-(((int)(cmShiftTable[3])))))
                             : ((((usqInt)((sourceWord & (cmMaskTable[3])))
                                  << (((int)(cmShiftTable[3]))))))));

            /* avoid introducing transparency by color reduction */
            if ((pv == 0) && (sourceWord != 0)) {
              pv = 1;
            }
          }
          if (mapperFlags & ColorMapIndexedPart) {
            pv = cmLookupTable[pv & cmMask];
          }
        }
        sourceWord = pv;
        sourceWord = (((usqInt)(sourceWord) << srcShift));

        /* Store back */

        /* begin dstLongAt:put:mask: */
        /* begin dstLongAt: */
        assert((((usqInt)dstIndex)) < endOfDestination);
        dstValue = long32At((void *)(dstIndex));
        dstValue = dstValue & dstMask;
        dstValue = dstValue | sourceWord;

        /* dstLongAt:put: */
        long32Atput((void *)(dstIndex), dstValue);
      }

      /* Everything below 31 is transparent */
      srcIndex += 4;
      if (destMSB) {
        if (srcShift) {
          srcShift -= 8;
          dstMask = (((usqInt)(dstMask)) >> 8) | 0xFF000000U;
        } else {
          dstIndex += 4;
          srcShift = 24;
          dstMask = 0xFFFFFF;
        }
      } else {
        if (srcShift == 24) {
          dstIndex += 4;
          srcShift = 0;
          dstMask = 0xFFFFFF00U;
        } else {
          srcShift += 8;
          dstMask = ((((usqInt)(dstMask) << 8))) | 0xFF;
        }
      }
      adjust = adjust ^ 0x1F1F1F1F;
    }
    srcY += 1;
    dstY += 1;
  }
  return 0;
}