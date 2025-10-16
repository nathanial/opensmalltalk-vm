/* Extracted from interp.c:45592 (function shouldRemapOop). */

sqInt
shouldRemapOop(sqInt oop)
{
	return ((!(oop & (tagMask()))))
	 && (shouldRemapObj(oop));
}