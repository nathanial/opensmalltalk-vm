/* Extracted from B2DPlugin.c:594 (function accurateLengthOfwith). */

static sqInt accurateLengthOfwith(sqInt deltaX, sqInt deltaY) {
  sqInt length2;

  if (!deltaX) {
    if (deltaY < 0) {
      return 0 - deltaY;
    } else {
      return deltaY;
    }
  }
  if (!deltaY) {
    if (deltaX < 0) {
      return 0 - deltaX;
    } else {
      return deltaX;
    }
  }
  length2 = (deltaX * deltaX) + (deltaY * deltaY);

  /* begin computeSqrt: */
  if (length2 < 32) {
    return (smallSqrtTable())[length2];
  } else {
    return ((sqInt)((sqrt(((double)length2))) + 0.5));
  }
}