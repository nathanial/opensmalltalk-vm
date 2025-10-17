/* Extracted from interp.c:49082 (function allocateOrExtendSegmentInfos). */

/*	Increase the number of allocated segInfos by 16. */

	/* SpurSegmentManager>>#allocateOrExtendSegmentInfos */

static void
allocateOrExtendSegmentInfos(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt newNumSegs;

	if (!GIV(numSegInfos)) {
		GIV(numSegInfos) = 16;
		GIV(segments) = calloc(GIV(numSegInfos), sizeof(SpurSegmentInfo));
		return;
	}
	newNumSegs = GIV(numSegInfos) + 16;
	GIV(segments) = realloc(GIV(segments), newNumSegs * (sizeof(SpurSegmentInfo)));
	if (!GIV(segments)) {
		error("out of memory; cannot allocate more segments");
	}
	memset(GIV(segments) + GIV(numSegInfos), 0, (newNumSegs - GIV(numSegInfos)) * (sizeof(SpurSegmentInfo)));
	GIV(numSegInfos) = newNumSegs;
}