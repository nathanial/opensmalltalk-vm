/* Extracted from B2DPlugin.c:10857 (function topRightX). */

static int topRightX(void) {
  if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
    return workBuffer[(workBuffer[GWBufferTop]) +
                      ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                        3 /* stackFillEntryLength */) +
                       2)];
  } else {
    return 999999999;
  }
}