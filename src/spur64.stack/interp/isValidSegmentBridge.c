/* Extracted from interp.c:49270 (function isValidSegmentBridge). */

static NoDbgRegParms sqInt
isValidSegmentBridge(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((addressCouldBeObj(objOop))
	 || (objOop == GIV(endOfMemory)))
	 && ((((longAt((void *)(objOop))) & (classIndexMask())) == (segmentBridgePun()))
	 && (((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) == (numSlotsMask()))
	 || ((numSlotsOfAny(objOop)) == 0)));
}