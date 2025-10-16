/* Extracted from interp.c:37311 (function isFreeObject). */

static NoDbgRegParms int
isFreeObject(sqInt objOop)
{
	return ((longAt((void *)(objOop))) & (classIndexMask())) == (isFreeObjectClassIndexPun());
}