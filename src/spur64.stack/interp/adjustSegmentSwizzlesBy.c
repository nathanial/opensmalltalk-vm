/* Extracted from interp.c:49037 (function adjustSegmentSwizzlesBy). */

/*	Adjust swizzles by firstSegmentShift. Also computes segStarts as
	they were in the image when it was written, so that oops' segments
	can be determined and hence oops correctly swizzled. */

	/* SpurSegmentManager>>#adjustSegmentSwizzlesBy: */

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