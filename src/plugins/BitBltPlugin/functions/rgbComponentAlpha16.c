/* Extracted from BitBltPlugin.c:4933 (function rgbComponentAlpha16). */

static sqInt rgbComponentAlpha16(void) {
  sqInt addThreshold;
  int deltaX;
  int deltaY;
  unsigned int destWord;
  unsigned int ditherBase;
  unsigned int ditherIndex;
  int ditherThreshold;
  sqInt dstIndex;
  unsigned int dstMask;
  sqInt dstValue;
  sqInt dstY;
  unsigned int sourceWord;
  unsigned int srcAlpha;
  sqInt srcIndex;
  unsigned int srcShift;
  sqInt srcY;

  /* So we can pre-decrement */
  deltaY = bbH + 1;
  srcY = sy;
  dstY = dy;
  srcShift = (dx & 1) * 16;
  if (destMSB) {
    srcShift = 16 - srcShift;
  }
  mask1 = 0xFFFFU << (16 - srcShift);

  /* This is the outer loop */
  while (((deltaY -= 1)) != 0) {
    srcIndex = (sourceBits + (srcY * sourcePitch)) + (sx * 4);
    dstIndex = (destBits + (dstY * destPitch)) + ((dx / 2) * 4);
    ditherBase = (dstY & 3) * 4;

    /* For pre-increment */
    ditherIndex = (sx & 3) - 1;

    /* So we can pre-decrement */
    deltaX = bbW + 1;
    dstMask = mask1;
    if (dstMask == 0xFFFF) {
      srcShift = 16;
    } else {
      srcShift = 0;
    }

    /* This is the inner loop */
    while (((deltaX -= 1)) != 0) {
      ditherThreshold =
          ditherMatrix4x4[ditherBase + ((ditherIndex = (ditherIndex + 1) & 3))];

      /* begin srcLongAt: */
      assert((((usqInt)srcIndex)) < endOfSource);
      sourceWord = long32At((void *)(srcIndex));
      srcAlpha = sourceWord & 0xFFFFFF;
      if (srcAlpha) {
        /* begin dstLongAt: */
        assert((((usqInt)dstIndex)) < endOfDestination);
        destWord = long32At((void *)(dstIndex));
        destWord = destWord & ((unsigned int)~dstMask);
        destWord = ((usqInt)(destWord)) >> srcShift;

        /* Expand from 16 to 32 bit by adding zero bits */
        destWord = ((((((usqInt)((destWord & 0x7C00)) << 9)))) |
                    (((((usqInt)((destWord & 0x3E0)) << 6))))) |
                   ((((((usqInt)((destWord & 0x1F)) << 3)))) | 0xFF000000U);

        /* Mix colors */
        sourceWord = rgbComponentAlpha32with(sourceWord, destWord);

        /* And dither */

        /* begin dither32To16:threshold: */
        addThreshold = (((sqInt)((usqInt)(ditherThreshold) << 8)));
        sourceWord = ((
            unsigned int)(((((((usqInt)((
                                   dither8Lookup[addThreshold +
                                                 (((((usqInt)(sourceWord)) >>
                                                    16)) &
                                                  0xFF)]))
                               << 10)))) +
                           (((((usqInt)((dither8Lookup
                                             [addThreshold +
                                              (((((usqInt)(sourceWord)) >> 8)) &
                                               0xFF)]))
                               << 5))))) +
                          (dither8Lookup[addThreshold + (sourceWord & 0xFF)])));
        if (sourceWord) {
          sourceWord = (((usqInt)(sourceWord) << srcShift));
        } else {
          sourceWord = 1U << srcShift;
        }

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

      /* 0 < srcAlpha
         If we have to mix colors then just copy a single word */
      srcIndex += 4;
      if (destMSB) {
        if (!srcShift) {
          dstIndex += 4;
        }
      } else {
        if (srcShift) {
          dstIndex += 4;
        }
      }

      /* Toggle between 0 and 16 */
      srcShift = srcShift ^ 16;

      /* Mask other half word */
      dstMask = (unsigned int)~dstMask;
    }
    srcY += 1;
    dstY += 1;
  }
  return 0;
}