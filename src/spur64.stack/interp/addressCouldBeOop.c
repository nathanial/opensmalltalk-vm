/* Extracted from interp.c:29754 (function addressCouldBeOop). */

/*	Answer if address appears to be that of either an immediate or an object.
	For code disassembly and assertions. */

	/* SpurMemoryManager>>#addressCouldBeOop: */

sqInt
addressCouldBeOop(sqInt address)
{
	return (((address & (tagMask())) != 0))
	 || (addressCouldBeObj(address));
}