/* Extracted from interp.c:37318 (function isFreeOop). */

static NoDbgRegParms sqInt
isFreeOop(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (((longAt((void *)(oop))) & (classIndexMask())) == (isFreeObjectClassIndexPun()));
}