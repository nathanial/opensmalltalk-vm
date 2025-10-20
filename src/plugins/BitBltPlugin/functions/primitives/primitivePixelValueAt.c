/* Extracted from BitBltPlugin.c:4573 (function primitivePixelValueAt). */
/* BitBltSimulation>>#primitivePixelValueAtX:y: */
/*	Sets the single pixel at x@y. Answers the previous value of the pixel.
        It does not handle LSB bitmaps right now.
        If x or y are < 0, return 0 to indicate transparent (cf
        BitBlt>bitPeekerFromForm: usage).
        Likewise if x>width or y>depth.
        Fail if the rcvr doesn't seem to be a Form, or x|y seem wrong
 */

EXPORT(sqInt)
primitivePixelValueAt(void) {
  sqInt bitmap;
  sqInt bitsSize;
  sqInt depth;
  unsigned int mask;
  sqInt pixel;
  sqInt ppW;
  sqInt rcvr;
  sqInt shift;
  sqInt stride;
  sqInt word;
  sqInt xVal;
  sqInt yVal;
  sqInt _return_value;

  if (!((isIntegerObject((xVal = stackValue(1)))) &&
        (isIntegerObject((yVal = stackValue(0)))))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  xVal = integerValueOf(xVal);
  yVal = integerValueOf(yVal);
  rcvr = stackValue(2);

  /* possible quick exit if x or y is -ve */
  if ((xVal < 0) || (yVal < 0)) {
    _return_value = integerObjectOf(0);
    if (!(failed())) {
      methodReturnValue(_return_value);
    }
    return null;
  }

  /* check that rcvr is plausibly a Form or subclass */
  rcvr = stackValue(methodArgumentCount());
  if (!((isPointers(rcvr)) && ((slotSizeOf(rcvr)) >= 4))) {
    return primitiveFail();
  }

  /* get the bits oop and width/height/depth */
  bitmap = fetchPointerofObject(FormBitsIndex, rcvr);
  if (!(isWordsOrBytes(bitmap))) {
    return primitiveFail();
  }
  width = fetchIntegerofObject(FormWidthIndex, rcvr);
  height = fetchIntegerofObject(FormHeightIndex, rcvr);
  depth = fetchIntegerofObject(FormDepthIndex, rcvr);

  /* if width/height/depth are not integer, fail */
  if (failed()) {
    return null;
  }

  /* possible quick exit if x or y is >= extent of form. This also catches cases
   * where the width/height are < 0 */
  if ((xVal >= width) || (yVal >= height)) {
    _return_value = integerObjectOf(0);
    if (!(failed())) {
      methodReturnValue(_return_value);
    }
    return null;
  }

  /* we don't handle LSB Forms yet */
  if (depth < 0) {
    return primitiveFail();
  }

  /* OK so now we know we have a plausible Form, the width/height/depth/x/y are
   * all reasonable and it's time to plunder the bitmap */

  /* pixels in each word */
  ppW = 32 / depth;

  /* how many words per row of pixels */
  stride = (width + (ppW - 1)) / ppW;
  bitsSize = byteSizeOf(bitmap);
  if (!(bitsSize >= ((stride * height) * 4))) {
    return primitiveFail();
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
  pixel = (((usqInt)(word)) >> shift) & mask;
  if (!(failed())) {
    methodReturnValue(positive32BitIntegerFor(pixel));
  }
  return null;
}
