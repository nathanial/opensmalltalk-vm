/* Extracted from interp.c:29754 (function addressCouldBeOop). */

sqInt
addressCouldBeOop(sqInt address)
{
	return (((address & (tagMask())) != 0))
	 || (addressCouldBeObj(address));
}