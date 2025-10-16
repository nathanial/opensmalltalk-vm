/* Extracted from interp.c:37425 (function isLargeFreeObject). */

static NoDbgRegParms int
isLargeFreeObject(sqInt objOop)
{
	return (bytesInBody(objOop)) >= 0x200 /* (numFreeLists * #allocationUnit) */;
}