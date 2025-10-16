/* Extracted from interp.c:37142 (function isBridgeOrEnumerableObjectNoAssert). */

static NoDbgRegParms int
isBridgeOrEnumerableObjectNoAssert(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classIndex;

	classIndex = (longAt((void *)(objOop))) & (classIndexMask());
	return (classIndex >= (isForwardedObjectClassIndexPun())
			? classIndex < (GIV(numClassTablePages) * (1U << (classTableMajorIndexShift())))
			: classIndex == (segmentBridgePun()));
}