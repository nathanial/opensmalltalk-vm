/* Extracted from B2DPlugin.c:10834 (function topDepth). */

static int topDepth(void) {
  if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
    return workBuffer[(workBuffer[GWBufferTop]) +
                      ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                        3 /* stackFillEntryLength */) +
                       1)];
  } else {
    return -1;
  }
}