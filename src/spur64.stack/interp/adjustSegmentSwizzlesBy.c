/* Extracted from interp.c:49037 (function adjustSegmentSwizzlesBy). */

/*	Adjust swizzles by firstSegmentShift. Also computes segStarts as
        they were in the image when it was written, so that oops' segments
        can be determined and hence oops correctly swizzled. */

/* SpurSegmentManager>>#adjustSegmentSwizzlesBy: */

static NoDbgRegParms void adjustSegmentSwizzlesBy(sqInt firstSegmentShift) {
  sqInt i;
  sqInt oldBaseAddr;
  SpurSegmentInfo *segInfo;

  oldBaseAddr = oldSpaceStart - firstSegmentShift;
  for (i = 0; i < numSegments; i += 1) {
    segInfo = (&(segments[i]));
    (segInfo->segStart = ((segInfo->segStart)) + oldBaseAddr);
    (segInfo->swizzle = ((segInfo->swizzle)) - oldBaseAddr);
  }
  canSwizzle = 1;
}