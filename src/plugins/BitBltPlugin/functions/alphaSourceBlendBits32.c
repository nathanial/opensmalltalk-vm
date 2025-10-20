/* Extracted from BitBltPlugin.c:953 (function alphaSourceBlendBits32). */
/* BitBltSimulation>>#alphaSourceBlendBits32 */
/*	This version assumes
        combinationRule = 34
        sourcePixSize = 32
        destPixSize = 8
        sourceForm ~= destForm.
        Note: This is not real blending since we don't have the source colors
        available.  */

static sqInt alphaSourceBlendBits32(void) {
  int deltaX;
  int deltaY;
  unsigned int destWord;
  sqInt dstIndex;
  sqInt dstY;
  unsigned int sourceWord;
  unsigned int srcAlpha;
  sqInt srcIndex;
  sqInt srcY;

  /* This particular method should be optimized in itself */
  /* Give the compile a couple of hints */
  /* The following should be declared as pointers so the compiler will
     notice that they're used for accessing memory locations
     (good to know on an Intel architecture) but then the increments
     would be different between ST code and C code so must hope the
     compiler notices what happens (MS Visual C does) */
  /* So we can pre-decrement */
  deltaY = bbH + 1;
  srcY = sy;
  dstY = dy;

  /* This is the outer loop */
  while (((deltaY -= 1)) != 0) {
    srcIndex = (sourceBits + (srcY * sourcePitch)) + (sx * 4);
    dstIndex = (destBits + (dstY * destPitch)) + (dx * 4);

    /* So we can pre-decrement */
    /* This is the inner loop */
    deltaX = bbW + 1;
    while (((deltaX -= 1)) != 0) {
      /* begin srcLongAt: */
      assert((((usqInt)srcIndex)) < endOfSource);
      sourceWord = long32At((void *)(srcIndex));
      srcAlpha = ((usqInt)(sourceWord)) >> 24;
      if (srcAlpha == 0xFF) {
        /* dstLongAt:put: */
        long32Atput((void *)(dstIndex), sourceWord);
        srcIndex += 4;
        dstIndex += 4;

        /* Now copy as many words as possible with alpha = 255 */
        while ((((deltaX -= 1)) != 0) &&
               ((((usqInt)(((/* begin srcLongAt: */
                             assert((((usqInt)srcIndex)) < endOfSource),
                             (sourceWord = long32At((void *)(srcIndex))))))) >>
                 24) == 0xFF)) {
          /* dstLongAt:put: */
          long32Atput((void *)(dstIndex), sourceWord);
          srcIndex += 4;
          dstIndex += 4;
        }

        /* Adjust deltaX */
        deltaX += 1;
      } else {
        if (srcAlpha) {
          /* begin dstLongAt: */
          assert((((usqInt)dstIndex)) < endOfDestination);
          destWord = long32At((void *)(dstIndex));
          destWord = alphaBlendScaledwith(sourceWord, destWord);

          /* dstLongAt:put: */
          long32Atput((void *)(dstIndex), destWord);
          srcIndex += 4;
          dstIndex += 4;
        } else {
          srcIndex += 4;
          dstIndex += 4;

          /* Now skip as many words as possible, */
          while (
              (((deltaX -= 1)) != 0) &&
              ((((usqInt)(((/* begin srcLongAt: */
                            assert((((usqInt)srcIndex)) < endOfSource),
                            (sourceWord = long32At((void *)(srcIndex))))))) >>
                24) == 0)) {
            srcIndex += 4;
            dstIndex += 4;
          }

          /* Adjust deltaX */
          deltaX += 1;
        }
      }
    }
    srcY += 1;
    dstY += 1;
  }
  return 0;
}
