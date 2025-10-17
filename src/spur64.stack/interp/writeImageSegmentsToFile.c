/* Extracted from interp.c:49782 (function writeImageSegmentsToFile). */

	/* SpurSegmentManager>>#writeImageSegmentsToFile: */

static NoDbgRegParms sqInt
writeImageSegmentsToFile(sqImageFile aBinaryStream)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt total;

	assert(((GIV(endOfMemory)) == (segLimit(&GIV(segments)[GIV(numSegments) - 1])))
	 || (((GIV(endOfMemory)) + (bridgeSize())) == (segLimit(&GIV(segments)[GIV(numSegments) - 1]))));
	if (GIV(firstSegmentSize)) {
		assert(GIV(firstSegmentSize) == (((GIV(segments)[0]).segSize)));
	}
	assert((((GIV(segments)[0]).segSize)) > 0);
	total = 0;
	for (i = 0; i < GIV(numSegments); i += 1) {
		if ((((GIV(segments)[i]).segSize)) > (2 * BaseHeaderSize)) {
			total += writeSegmentnextSegmenttoFile((&(GIV(segments)[i])), nextNonEmptySegmentAfter(i), aBinaryStream);
		}
	}
	return total;
}