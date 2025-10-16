/* Extracted from interp.c:37639 (function isUnambiguouslyForwarder). */

static NoDbgRegParms int
isUnambiguouslyForwarder(sqInt objOop)
{
	return ((longAt((void *)(objOop))) & (classIndexMask())) == (isForwardedObjectClassIndexPun());
}