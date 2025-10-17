/* Extracted from interp.c:37425 (function isLargeFreeObject). */

	/* SpurMemoryManager>>#isLargeFreeObject: */

static NoDbgRegParms int
isLargeFreeObject(sqInt objOop)
{
	return (bytesInBody(objOop)) >= 0x200 /* (numFreeLists * #allocationUnit) */;
}