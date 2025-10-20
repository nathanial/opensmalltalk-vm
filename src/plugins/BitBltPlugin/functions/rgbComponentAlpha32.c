/* Extracted from BitBltPlugin.c:5073 (function rgbComponentAlpha32). */

static sqInt rgbComponentAlpha32(void) {
  register sqInt deltaX;
  int deltaY;
  unsigned int destWord;
  register sqIntptr_t dstIndex;
  sqInt dstY;
  unsigned int sourceWord;
  unsigned int srcAlpha;
  register sqIntptr_t srcIndex;
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
      srcAlpha = sourceWord & 0xFFFFFF;
      if (srcAlpha) {
        /* begin dstLongAt: */
        assert((((usqInt)dstIndex)) < endOfDestination);
        destWord = long32At((void *)(dstIndex));
        destWord = rgbComponentAlpha32with(sourceWord, destWord);

        /* dstLongAt:put: */
        long32Atput((void *)(dstIndex), destWord);
        srcIndex += 4;
        dstIndex += 4;
      } else {
        srcIndex += 4;
        dstIndex += 4;

        /* Now skip as many words as possible, */
        while ((((deltaX -= 1)) != 0) &&
               ((((/* begin srcLongAt: */
                   assert((((usqInt)srcIndex)) < endOfSource),
                   (sourceWord = long32At((void *)(srcIndex))))) &
                 0xFFFFFF) == 0)) {
          srcIndex += 4;
          dstIndex += 4;
        }

        /* Adjust deltaX */
        deltaX += 1;
      }
    }
    srcY += 1;
    dstY += 1;
  }
  return 0;
}