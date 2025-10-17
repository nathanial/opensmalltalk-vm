/* Extracted from interp.c:37519 (function isOopForwarded). */

	/* SpurMemoryManager>>#isOopForwarded: */

sqInt
isOopForwarded(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))));
}