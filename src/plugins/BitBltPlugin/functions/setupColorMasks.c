/* Extracted from BitBltPlugin.c:5868 (function setupColorMasks). */

static sqInt setupColorMasks(void) {
  sqInt bits;
  sqInt targetBits;

  bits = (targetBits = 0);
  if (sourceDepth <= 8) {
    return null;
  }
  if (sourceDepth == 16) {
    bits = 5;
  }
  if (sourceDepth == 32) {
    bits = 8;
  }
  if (cmBitsPerColor) {
    targetBits = cmBitsPerColor;
  } else {
    if (destDepth <= 8) {
      return null;
    }
    if (destDepth == 16) {
      targetBits = 5;
    }
    if (destDepth == 32) {
      targetBits = 8;
    }
  }

  /* Convert to destDepth */
  setupColorMasksFromto(bits, targetBits);
  return 0;
}