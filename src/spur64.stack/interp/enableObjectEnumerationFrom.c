/* Extracted from interp.c:34598 (function enableObjectEnumerationFrom). */

/*	We use bridges to stitch segments together to make it appear that the
   heap is one contiguous space. Bridges at the end of oldSpace segments are
   maintained. Bridges at the end of pastSpace and eden are temporary, and are
   established here, depending on the current sizes of pastSpace end eden.

        N.B. this introduces complications. Either or both pastSpace and eden
   may be empty, so the bridge from pastSpace may skip eden. pastSpace may be
   full, so there may be no bridge at the end of pastSpace. Most difficult,
   pastSpace could be one 64-bit word short of full, but normal bridges are two
   word objects. To make this work we introduce a hack,
        objectAfterMaybeSlimBridge:limit:, which uses a fake overflow slot count
        to get to the start of the next object, which is either one or two
        words away, depending on whether the first object in eden has a normal
   or an overflow header. */

/* SpurMemoryManager>>#enableObjectEnumerationFrom: */

static NoDbgRegParms void enableObjectEnumerationFrom(sqInt initialObject) {
  if (oopisLessThan(initialObject, oldSpaceStart)) {
    if (freeStart > (((eden).start))) {
      /* begin bridgeEdenAndOldSpace */
      initSegmentBridgeWithBytesat(oldSpaceStart - freeStart, freeStart);
      if (pastSpaceStart < (((eden).start))) {
        if ((pastSpaceStart + BaseHeaderSize) == (((eden).start))) {
          hackSlimBridgeToat(objectStartingAt(((eden).start)), pastSpaceStart);

          /* And carefully check the assumption */
          assert((objectAfterMaybeSlimBridgelimit(
                     objectInPastSpaceBefore(pastSpaceStart), nilObj)) ==
                 (objectStartingAt(((eden).start))));
        } else {
          initSegmentBridgeWithBytesat((((eden).start)) - pastSpaceStart,
                                       pastSpaceStart);
        }
      }
    } else {
      /* begin bridgePastSpaceAndOldSpace */
      initSegmentBridgeWithBytesat(oldSpaceStart - pastSpaceStart,
                                   pastSpaceStart);
    }
  }
}