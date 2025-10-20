/* Extracted from BitBltPlugin.c:4684 (function primitivePixelValueAtPut). */

EXPORT(sqInt)
primitivePixelValueAtPut(void) {
  sqInt bitmap;
  sqInt bitsSize;
  sqInt depth;
  unsigned int mask;
  sqInt oldPixel;
  sqInt pixel;
  sqInt ppW;
  sqInt rcvr;
  sqInt shift;
  sqInt stride;
  sqInt word;
  sqInt xVal;
  sqInt yVal;

  if (!((isIntegerObject((xVal = stackValue(2)))) &&
        ((isIntegerObject((yVal = stackValue(1)))) &&
         (isIntegerObject((pixel = stackValue(0))))))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  xVal = integerValueOf(xVal);
  yVal = integerValueOf(yVal);
  pixel = integerValueOf(pixel);
  rcvr = stackValue(3);

  /* check that rcvr is plausibly a Form or subclass */
  rcvr = stackValue(methodArgumentCount());
  if (!((isPointers(rcvr)) && ((slotSizeOf(rcvr)) >= 4))) {
    return primitiveFailFor(PrimErrBadReceiver);
  }

  /* get the bits oop and width/height/depth */
  bitmap = fetchPointerofObject(FormBitsIndex, rcvr);
  if (!(isWordsOrBytes(bitmap))) {
    return primitiveFailFor(PrimErrBadReceiver);
  }
  width = fetchIntegerofObject(FormWidthIndex, rcvr);
  height = fetchIntegerofObject(FormHeightIndex, rcvr);
  depth = fetchIntegerofObject(FormDepthIndex, rcvr);

  /* if width/height/depth are not integer, fail */
  if ((failed()) || (depth < 0)) {
    return primitiveFailFor(PrimErrBadReceiver);
  }
  if ((xVal < 0) ||
      ((xVal >= width) || ((yVal < 0) || ((yVal >= height) || (pixel < 0))))) {
    return primitiveFailFor(PrimErrBadArgument);
  }

  /* OK so now we know we have a plausible Form, the width/height/depth/x/y are
   * all reasonable and it's time to plunder the bitmap */

  /* pixels in each word */
  ppW = 32 / depth;

  /* how many words per row of pixels */
  stride = (width + (ppW - 1)) / ppW;
  bitsSize = byteSizeOf(bitmap);
  if (!(bitsSize >= ((stride * height) * 4))) {
    return primitiveFailFor(PrimErrBadReceiver);
  }

  /* bytes per word */

  /* load the word that contains our target */
  word = fetchLong32ofObject((yVal * stride) + (xVal / ppW), bitmap);

  /* make a mask to isolate the pixel within that word */
  mask = ((usqInt)(0xFFFFFFFFU)) >> (32 - depth);

  /* this is the tricky MSB part - we mask the xVal to find how far into the
   * word we need, then add 1 for the pixel we're looking for, then * depth to
   * get the bit shift */
  shift = 32 - (((xVal & (ppW - 1)) + 1) * depth);

  /* shift, mask and dim the lights */
  oldPixel = (((usqInt)(word)) >> shift) & mask;
  word =
      ((word | ((((usqInt)(mask) << shift)))) - ((((usqInt)(mask) << shift)))) +
      ((((usqInt)(pixel) << shift)));
  storeLong32ofObjectwithValue((yVal * stride) + (xVal / ppW), bitmap, word);
  if (!(failed())) {
    methodReturnValue(positive32BitIntegerFor(oldPixel));
  }
  return null;
}