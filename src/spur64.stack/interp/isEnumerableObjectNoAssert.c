/* Extracted from interp.c:37247 (function isEnumerableObjectNoAssert). */

/*	Answer if objOop should be included in an allObjects...Do: enumeration.
	This is for assert-checking only. */

	/* SpurMemoryManager>>#isEnumerableObjectNoAssert: */

static NoDbgRegParms sqInt
isEnumerableObjectNoAssert(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classIndex;

	classIndex = (longAt((void *)(objOop))) & (classIndexMask());
	return (classIndex >= (isForwardedObjectClassIndexPun()))
	 && (classIndex < (GIV(numClassTablePages) * (1U << (classTableMajorIndexShift()))));
}