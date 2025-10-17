/* Extracted from interp.c:37142 (function isBridgeOrEnumerableObjectNoAssert). */

/*	Answer if objOop should be included in an allObjects...Do: enumeration.
	This is for assert-checking only. */

	/* SpurMemoryManager>>#isBridgeOrEnumerableObjectNoAssert: */

static NoDbgRegParms int
isBridgeOrEnumerableObjectNoAssert(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classIndex;

	classIndex = (longAt((void *)(objOop))) & (classIndexMask());
	return (classIndex >= (isForwardedObjectClassIndexPun())
			? classIndex < (numClassTablePages * (1U << (classTableMajorIndexShift())))
			: classIndex == (segmentBridgePun()));
}