/* Extracted from interp.c:46262 (function topRemappableOop). */

sqInt
topRemappableOop(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(remapBuffer)[GIV(remapBufferCount)];
}