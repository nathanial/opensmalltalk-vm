/* Extracted from B2DPlugin.c:10846 (function topFill). */

static int topFill(void) {
  if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
    return workBuffer[(workBuffer[GWBufferTop]) +
                      (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                       3 /* stackFillEntryLength */)];
  } else {
    return 0;
  }
}