/* Extracted from interp.c:49809 (function writeSegmentnextSegmenttoFile). */

/*	Write the segment contents, the size of and the distance to the next
        segment to aBinaryStream.
 */

/* SpurSegmentManager>>#writeSegment:nextSegment:toFile: */

static sqInt writeSegmentnextSegmenttoFile(
    SpurSegmentInfo *segment, SpurSegmentInfo *nextSegment,
    sqImageFile aBinaryStream) {
  usqLong firstSavedBridgeWord;
  sqInt nWritten;
  sqInt pier1;
  sqInt pier2;
  usqLong secondSavedBridgeWord;

  pier1 = (((segment->segSize)) + ((segment->segStart))) - (2 * BaseHeaderSize);
  pier2 = pier1 + BaseHeaderSize;
  assert(isValidSegmentBridge(bridgeFor(segment)));
  assert((startOfObject(bridgeFor(segment))) == pier1);

  /* Temporarily change the bridge to bridge to the next non-empty segment.
     The first double word of the bridge includes the bridge size in slots, and
     hence specifies the distance to the next segment. The following double
     word is replaced by the size of the next segment, or 0 if there isn't one.
   */
  firstSavedBridgeWord = long64At((void *)(pier1));
  secondSavedBridgeWord = long64At((void *)(pier2));
  bridgeFromto(segment, nextSegment);
  long64Atput((void *)(pier2), (nextSegment ? (nextSegment->segSize) : 0));
  nWritten = sqImageFileWrite(((void *)((segment->segStart))), 1,
                              (segment->segSize), aBinaryStream);
  long64Atput((void *)(pier1), firstSavedBridgeWord);
  long64Atput((void *)(pier2), secondSavedBridgeWord);
  return nWritten;
}