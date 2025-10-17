/* Extracted from interp.c:48954 (function segLimit). */

/* SpurSegmentInfo>>#segLimit */

static NoDbgRegParms usqInt segLimit(SpurSegmentInfo *self_in_SpurSegmentInfo) {
  return ((self_in_SpurSegmentInfo->segSize)) +
         ((self_in_SpurSegmentInfo->segStart));
}