/* Extracted from interp.c:49082 (function allocateOrExtendSegmentInfos). */

/*	Increase the number of allocated segInfos by 16. */

/* SpurSegmentManager>>#allocateOrExtendSegmentInfos */

static void allocateOrExtendSegmentInfos(void) {
  sqInt newNumSegs;

  if (!numSegInfos) {
    numSegInfos = 16;
    segments = calloc(numSegInfos, sizeof(SpurSegmentInfo));
    return;
  }
  newNumSegs = numSegInfos + 16;
  segments = realloc(segments, newNumSegs * (sizeof(SpurSegmentInfo)));
  if (!segments) {
    error("out of memory; cannot allocate more segments");
  }
  memset(segments + numSegInfos, 0,
         (newNumSegs - numSegInfos) * (sizeof(SpurSegmentInfo)));
  numSegInfos = newNumSegs;
}