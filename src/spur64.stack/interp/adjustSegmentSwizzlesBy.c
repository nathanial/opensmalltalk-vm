/* Extracted from interp.c:49037 (function adjustSegmentSwizzlesBy). */

static NoDbgRegParms void
adjustSegmentSwizzlesBy(sqInt firstSegmentShift)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt oldBaseAddr;
    SpurSegmentInfo *segInfo;

	oldBaseAddr = GIV(oldSpaceStart) - firstSegmentShift;
	for (i = 0; i < GIV(numSegments); i += 1) {
		segInfo = (&(GIV(segments)[i]));
		(segInfo->segStart = ((segInfo->segStart)) + oldBaseAddr);
		(segInfo->swizzle = ((segInfo->swizzle)) - oldBaseAddr);
	}
	GIV(canSwizzle) = 1;
}