/* Extracted from BitBltPlugin.c:5906 (function setupColorMasksFromto). */

static sqInt setupColorMasksFromto(sqInt srcBits, sqInt targetBits) {
  sqInt deltaBits;
  sqInt mask;
  static unsigned int masks[4] = {0, 0, 0, 0};
  static int shifts[4] = {0, 0, 0, 0};

  deltaBits = targetBits - srcBits;
  if (!deltaBits) {
    return 0;
  }
  if (deltaBits <= 0) {
    mask = (1U << targetBits) - 1;

    /* Mask for extracting a color part of the source */
    masks[RedIndex] =
        (((sqInt)((usqInt)(mask) << ((srcBits * 2) - deltaBits))));
    masks[GreenIndex] = (((sqInt)((usqInt)(mask) << (srcBits - deltaBits))));
    masks[BlueIndex] = (((sqInt)((usqInt)(mask) << (0 - deltaBits))));
    masks[AlphaIndex] = 0;
  } else {
    mask = (1U << srcBits) - 1;

    /* Mask for extracting a color part of the source */
    masks[RedIndex] = (((sqInt)((usqInt)(mask) << (srcBits * 2))));
    masks[GreenIndex] = (((sqInt)((usqInt)(mask) << srcBits)));
    masks[BlueIndex] = mask;
  }

  /* Shifts for adjusting each value in a cm RGB value */
  shifts[RedIndex] = (deltaBits * 3);
  shifts[GreenIndex] = (deltaBits * 2);
  shifts[BlueIndex] = deltaBits;
  shifts[AlphaIndex] = 0;
  cmShiftTable = shifts;
  cmMaskTable = masks;
  cmFlags = cmFlags | (ColorMapPresent | ColorMapFixedPart);
  return 0;
}