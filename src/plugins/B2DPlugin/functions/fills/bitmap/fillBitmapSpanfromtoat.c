/* Extracted from B2DPlugin.c:1933 (function fillBitmapSpanfromtoat). */

static sqInt fillBitmapSpanfromtoat(sqInt bmFill, sqInt leftX, sqInt rightX,
                                    sqInt yValue) {
  sqInt a;
  usqInt b;
  int *bits;
  int bmDepth;
  int bmHeight;
  int bmRaster;
  int bmWidth;
  sqInt cMask;
  sqInt deltaX;
  sqInt deltaY;
  sqInt ds;
  int dsX;
  sqInt dt;
  int dtX;
  sqInt fillValue;
  usqInt g;
  sqInt newDelta;
  sqInt newDeltaSqInt;
  usqInt r;
  int rShift;
  int tileFlag;
  int value;
  sqInt x;
  sqInt x1;
  sqInt xp;
  sqInt yp;

  if (!((workBuffer[GWAALevel]) == 1)) {
    return fillBitmapSpanAAfromtoat(bmFill, leftX, rightX, yValue);
  }
  bits = loadBitsFrom(bmFill);
  if (!bits) {
    return null;
  }
  bmWidth = objBuffer[bmFill + GBBitmapWidth];
  bmHeight = objBuffer[bmFill + GBBitmapHeight];
  tileFlag = (objBuffer[bmFill + GBTileFlag]) == 1;
  deltaX = leftX - (objBuffer[bmFill + GFOriginX]);
  deltaY = yValue - (objBuffer[bmFill + GFOriginY]);
  dsX = objBuffer[bmFill + GFDirectionX];
  dtX = objBuffer[bmFill + GFNormalX];
  ds = (deltaX * dsX) + (deltaY * (objBuffer[bmFill + GFDirectionY]));
  dt = (deltaX * dtX) + (deltaY * (objBuffer[bmFill + GFNormalY]));
  x = leftX;
  x1 = rightX;
  while (x < x1) {
    if (tileFlag) {
      /* begin repeatValue:max: */
      newDelta = ds;
      while (newDelta < 0) {
        newDelta += (((usqInt)(bmWidth) << 16));
      }
      while (newDelta >= ((((usqInt)(bmWidth) << 16)))) {
        newDelta -= (((usqInt)(bmWidth) << 16));
      }
      ds = newDelta;

      /* begin repeatValue:max: */
      newDeltaSqInt = dt;
      while (newDeltaSqInt < 0) {
        newDeltaSqInt += (((usqInt)(bmHeight) << 16));
      }
      while (newDeltaSqInt >= ((((usqInt)(bmHeight) << 16)))) {
        newDeltaSqInt -= (((usqInt)(bmHeight) << 16));
      }
      dt = newDeltaSqInt;
    }
    xp = ds / 0x10000;
    yp = dt / 0x10000;
    if (!tileFlag) {
      /* begin clampValue:max: */
      if (xp < 0) {
        xp = 0;
      } else {
        if (xp >= bmWidth) {
          xp = bmWidth - 1;
        } else {
        }
      }

      /* begin clampValue:max: */
      if (yp < 0) {
        yp = 0;
      } else {
        if (yp >= bmHeight) {
          yp = bmHeight - 1;
        } else {
        }
      }
    }
    if ((xp >= 0) && ((yp >= 0) && ((xp < bmWidth) && (yp < bmHeight)))) {
      /* begin bitmapValue:bits:atX:y: */
      bmDepth = objBuffer[bmFill + GBBitmapDepth];
      bmRaster = objBuffer[bmFill + GBBitmapRaster];
      if (bmDepth == 32) {
        value = (((int *)bits))[(bmRaster * yp) + xp];
        if ((value != 0) && ((value & 0xFF000000U) == 0)) {
          value = value | 0xFF000000U;
        }
        fillValue = uncheckedTransformColor(value);
      } else {
        /* rShift - shift value to convert from pixel to word index */
        rShift = (rShiftTable())[bmDepth];
        value = (((int *)bits))[(bmRaster * yp) + (((usqInt)(xp)) >> rShift)];

        /* cMask - mask out the pixel from the word */
        cMask = (1U << bmDepth) - 1;

        /* rShift - shift value to move the pixel in the word to the lowest bit
         * position */
        rShift = (32 - bmDepth) - ((xp & ((1U << rShift) - 1)) * bmDepth);
        value = (((usqInt)(value)) >> rShift) & cMask;
        if (bmDepth == 16) {
          if (value) {
            b = (((usqInt)((value & 0x1F)) << 3));
            b += (b) >> 5;
            g = (((usqInt)(((((usqInt)(value)) >> 5) & 0x1F)) << 3));
            g += (g) >> 5;
            r = (((usqInt)(((((usqInt)(value)) >> 10) & 0x1F)) << 3));
            r += (r) >> 5;
            a = 0xFF;
            value = ((b + ((g << 8))) + ((r << 16))) + ((((usqInt)(a) << 24)));
          }
        } else {
          if (objBuffer[bmFill + GBColormapSize]) {
            value = ((objBuffer + bmFill) + GBColormapOffset)[value];
          } else {
            value = 0;
          }
        }

        /* Must convert by expanding bits
           Must convert by using color map */
        fillValue = uncheckedTransformColor(value);
      }
      /* end bitmapValue:bits:atX:y: */
      spanBuffer[x] = fillValue;
    }
    ds += dsX;
    dt += dtX;
    x += 1;
  }
  return 0;
}