/* Extracted from interp.c:37357 (function isInEden). */

	/* SpurMemoryManager>>#isInEden: */

static NoDbgRegParms sqInt
isInEden(sqInt objOop)
{
	return oopisGreaterThanOrEqualToandLessThan(objOop, ((eden).start), freeStart);
}