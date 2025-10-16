/* Extracted from interp.c:37304 (function isForwarded). */

int
isForwarded(sqInt objOop)
{
	return (!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))));
}