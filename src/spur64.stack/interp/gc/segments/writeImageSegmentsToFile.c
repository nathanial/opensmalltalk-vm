/* Extracted from interp.c:49782 (function writeImageSegmentsToFile). */

/* SpurSegmentManager>>#writeImageSegmentsToFile: */

static sqInt writeImageSegmentsToFile(sqImageFile aBinaryStream) {
  sqInt i;
  sqInt total;

  assert(((endOfMemory) == (segLimit(&segments[numSegments - 1]))) ||
         (((endOfMemory) + (bridgeSize())) ==
          (segLimit(&segments[numSegments - 1]))));
  if (firstSegmentSize) {
    assert(firstSegmentSize == (((segments[0]).segSize)));
  }
  assert((((segments[0]).segSize)) > 0);
  total = 0;
  for (i = 0; i < numSegments; i += 1) {
    if ((((segments[i]).segSize)) > (2 * BaseHeaderSize)) {
      total += writeSegmentnextSegmenttoFile(
          (&(segments[i])), nextNonEmptySegmentAfter(i), aBinaryStream);
    }
  }
  return total;
}