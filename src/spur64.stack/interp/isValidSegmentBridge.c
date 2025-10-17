/* Extracted from interp.c:49270 (function isValidSegmentBridge). */

/*	bridges bridge the gaps between segments. They are the last object in
   each segment.
 */

/* SpurSegmentManager>>#isValidSegmentBridge: */

static NoDbgRegParms sqInt isValidSegmentBridge(sqInt objOop) {
  return ((addressCouldBeObj(objOop)) || (objOop == endOfMemory)) &&
         ((((longAt((void *)(objOop))) & (classIndexMask())) ==
           (segmentBridgePun())) &&
          (((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) ==
            (numSlotsMask())) ||
           ((numSlotsOfAny(objOop)) == 0)));
}