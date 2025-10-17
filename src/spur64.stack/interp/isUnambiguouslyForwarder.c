/* Extracted from interp.c:37639 (function isUnambiguouslyForwarder). */

/*	This version is private to SpurMemoryManager (for asserts, etc). It does
	not take advantage of the power-of-two optimization in isForwarded:. */

	/* SpurMemoryManager>>#isUnambiguouslyForwarder: */

static NoDbgRegParms int
isUnambiguouslyForwarder(sqInt objOop)
{
	return ((longAt((void *)(objOop))) & (classIndexMask())) == (isForwardedObjectClassIndexPun());
}