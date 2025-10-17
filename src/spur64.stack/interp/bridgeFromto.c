/* Extracted from interp.c:49137 (function bridgeFromto). */

/*	Create a bridge from aSegment to the next segment,
        or create a terminating bridge if there is no next segment. */

/* SpurSegmentManager>>#bridgeFrom:to: */

static void bridgeFromto(SpurSegmentInfo *aSegment,
                                       SpurSegmentInfo *nextSegmentOrNil) {
  sqInt bridgeSpan;
  sqInt clifton;
  usqInt segEnd;

  segEnd = ((aSegment->segSize)) + ((aSegment->segStart));

  /* clifton is where the Avon bridge begins... */
  clifton = segEnd - (2 * BaseHeaderSize);
  bridgeSpan = (nextSegmentOrNil ? (((nextSegmentOrNil->segStart)) - segEnd) +
                                       (2 * BaseHeaderSize)
                                 : 2 * BaseHeaderSize);
  assert(bridgeSpan >= 0);
  initSegmentBridgeWithBytesat(bridgeSpan, clifton);

  /* the revised bridge should get us to the new segment */
  assert((addressAfter(objectStartingAt(clifton))) ==
         ((nextSegmentOrNil == null ? segLimit(aSegment)
                                    : (nextSegmentOrNil->segStart))));
}