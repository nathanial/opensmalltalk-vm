/* Extracted from B2DPlugin.c:3419 (function findStackFilldepth). */

static sqInt findStackFilldepth(sqInt fillIndex, sqInt depth) {
  sqInt index;

  index = 0;
  while ((index < ((workBuffer[GWSize]) - (workBuffer[GWBufferTop]))) &&
         (((workBuffer[(workBuffer[GWBufferTop]) + index]) != fillIndex) ||
          ((workBuffer[(workBuffer[GWBufferTop]) + (index + 1)]) != depth))) {
    index += 3 /* stackFillEntryLength */;
  }
  if (index >= ((workBuffer[GWSize]) - (workBuffer[GWBufferTop]))) {
    return -1;
  } else {
    return index;
  }
}