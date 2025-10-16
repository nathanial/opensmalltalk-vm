/* Extracted from interp.c:42439 (function popRemappableOop). */

sqInt
popRemappableOop(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;

	oop = GIV(remapBuffer)[GIV(remapBufferCount)];
	GIV(remapBufferCount) -= 1;
	return oop;
}