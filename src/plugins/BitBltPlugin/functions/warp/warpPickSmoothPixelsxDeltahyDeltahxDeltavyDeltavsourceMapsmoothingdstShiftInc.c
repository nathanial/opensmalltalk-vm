/* Extracted from BitBltPlugin.c:6743 (function warpPickSmoothPixelsxDeltahyDeltahxDeltavyDeltavsourceMapsmoothingdstShiftInc). */
/* BitBltSimulation>>#warpPickSmoothPixels:xDeltah:yDeltah:xDeltav:yDeltav:sourceMap:smoothing:dstShiftInc: */
/*	Pick n (sub-) pixels from the source form, mapped by sourceMap,
        average the RGB values, map by colorMap and return the new word.
        This version is only called from WarpBlt with smoothingCount > 1 */

static sqInt
warpPickSmoothPixelsxDeltahyDeltahxDeltavyDeltavsourceMapsmoothingdstShiftInc(
    sqInt nPixels, sqInt xDeltah, sqInt yDeltah, sqInt xDeltav, sqInt yDeltav,
    sqInt sourceMap, sqInt n, sqInt dstShiftInc) {
  sqInt a;
  sqInt b;
  sqInt destWord;
  int dstMask;
  sqInt g;
  sqInt i;
  sqInt j;
  sqInt k;
  sqInt nPix;
  sqInt pv;
  sqInt r;
  unsigned int rgb;
  sqInt sourcePix;
  unsigned int sourceWord;
  sqInt srcIndex;
  sqInt val;
  sqInt x;
  sqInt xdh;
  sqInt xdv;
  sqInt xSqInt;
  sqInt xx;
  sqInt y;
  sqInt ydh;
  sqInt ydv;
  sqInt ySqInt;
  sqInt yy;

  /* nope - too much stuff in here */
  dstMask = maskTable[destDepth];
  destWord = 0;
  if (n == 2) {
    xdh = xDeltah / 2;
    ydh = yDeltah / 2;
    xdv = xDeltav / 2;
    ydv = yDeltav / 2;
  } else {
    xdh = xDeltah / n;
    ydh = yDeltah / n;
    xdv = xDeltav / n;
    ydv = yDeltav / n;
  }

  /* Try avoiding divides for most common n (divide by 2 is generated as shift)
   */
  i = nPixels;
  do {
    x = sx;
    y = sy;
    a = (r = (g = (b = 0)));

    /* Pick and average n*n subpixels */

    /* actual number of pixels (not clipped and not transparent) */
    nPix = 0;
    j = n;
    do {
      xx = x;
      yy = y;
      k = n;

      /* get a single subpixel */
      do {
        /* begin pickWarpPixelAtX:y: */
        if ((xx < 0) ||
            ((yy < 0) ||
             ((((xSqInt = ((usqInt)(xx)) >> BinaryPoint)) >= sourceWidth) ||
              (((ySqInt = ((usqInt)(yy)) >> BinaryPoint)) >= sourceHeight)))) {
          rgb = 0;
          goto l1;
        }
        srcIndex = (sourceBits + (ySqInt * sourcePitch)) +
                   ((((usqInt)(xSqInt)) >> warpAlignShift) * 4);

        /* begin srcLongAt: */
        assert((((usqInt)srcIndex)) < endOfSource);
        sourceWord = long32At((void *)(srcIndex));

        /* Extract pixel from word */
        srcBitShift = warpBitShiftTable[xSqInt & warpAlignMask];
        sourcePix = (((usqInt)(sourceWord)) >> srcBitShift) & warpSrcMask;
        rgb = ((unsigned int)sourcePix);
        /* end pickWarpPixelAtX:y: */
      l1:
        if (!((combinationRule == 25) && (rgb == 0))) {
          nPix += 1;
          if (sourceDepth < 16) {
            rgb = long32At((void *)(sourceMap + ((((usqInt)(rgb) << 2)))));
          } else {
            if (sourceDepth == 16) {
              rgb = (((((usqInt)((rgb & 0x1F)) << 3))) |
                     ((((usqInt)((rgb & 0x3E0)) << 6)))) |
                    ((((usqInt)((rgb & 0x7C00)) << 9)));
            } else {
            }
          }

          /* Get RGBA values from sourcemap table
             Already in RGB format */
          b += rgb & 0xFF;
          g += (((usqInt)(rgb)) >> 8) & 0xFF;
          r += (((usqInt)(rgb)) >> 16) & 0xFF;
          a += ((usqInt)(rgb)) >> 24;
        }

        /* PAINT
           If not clipped and not transparent, then tally rgb values */
        xx += xdh;
        yy += ydh;
      } while (!(((k -= 1)) == 0));
      x += xdv;
      y += ydv;
    } while (!(((j -= 1)) == 0));
    if ((nPix == 0) || ((combinationRule == 25) && (nPix < ((n * n) / 2)))) {
      /* All pixels were 0, or most were transparent */
      rgb = 0;
    } else {
      if (nPix == 4) {
        r = ((usqInt)(r)) >> 2;
        g = ((usqInt)(g)) >> 2;
        b = ((usqInt)(b)) >> 2;
        a = ((usqInt)(a)) >> 2;
      } else {
        r = r / nPix;
        g = g / nPix;
        b = b / nPix;
        a = a / nPix;
      }

      /* Try to avoid divides for most common n */
      rgb = ((((((usqInt)(a) << 24))) + ((((usqInt)(r) << 16)))) +
             ((((usqInt)(g) << 8)))) +
            b;

      /* map the pixel */
      if (!rgb) {
        if ((((r + g) + b) + a) > 0) {
          rgb = 1;
        }
      }

      /* only generate zero if pixel is really transparent */

      /* begin mapPixel:flags: */
      pv = rgb;
      if (cmFlags & ColorMapPresent) {
        if (cmFlags & ColorMapFixedPart) {
          /* begin rgbMapPixel:flags: */
          val = (((((int)(cmShiftTable[0]))) < 0)
                     ? (((usqInt)((rgb & (cmMaskTable[0])))) >>
                        (-(((int)(cmShiftTable[0])))))
                     : ((((usqInt)((rgb & (cmMaskTable[0])))
                          << (((int)(cmShiftTable[0])))))));
          val = val | ((((((int)(cmShiftTable[1]))) < 0)
                            ? (((usqInt)((rgb & (cmMaskTable[1])))) >>
                               (-(((int)(cmShiftTable[1])))))
                            : ((((usqInt)((rgb & (cmMaskTable[1])))
                                 << (((int)(cmShiftTable[1]))))))));
          val = val | ((((((int)(cmShiftTable[2]))) < 0)
                            ? (((usqInt)((rgb & (cmMaskTable[2])))) >>
                               (-(((int)(cmShiftTable[2])))))
                            : ((((usqInt)((rgb & (cmMaskTable[2])))
                                 << (((int)(cmShiftTable[2]))))))));
          pv = val | ((((((int)(cmShiftTable[3]))) < 0)
                           ? (((usqInt)((rgb & (cmMaskTable[3])))) >>
                              (-(((int)(cmShiftTable[3])))))
                           : ((((usqInt)((rgb & (cmMaskTable[3])))
                                << (((int)(cmShiftTable[3]))))))));

          /* avoid introducing transparency by color reduction */
          if ((pv == 0) && (rgb != 0)) {
            pv = 1;
          }
        }
        if (cmFlags & ColorMapIndexedPart) {
          pv = cmLookupTable[pv & cmMask];
        }
      }
      rgb = pv;
    }

    /* normalize rgba sums
       Mix it in */
    destWord = destWord | ((((usqInt)((rgb & dstMask)) << dstBitShift)));
    dstBitShift += dstShiftInc;
    sx += xDeltah;
    sy += yDeltah;
  } while (!(((i -= 1)) == 0));
  return destWord;
}
