/* Extracted from interp.c:46262 (function topRemappableOop). */

/*	Answers the top of the remappable oop stack. Useful when writing loops.
	We support this excessence for compatibility with ObjectMemory.
	Spur doesn't GC during allocation. */

	/* SpurMemoryManager>>#topRemappableOop */

sqInt
topRemappableOop(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(remapBuffer)[GIV(remapBufferCount)];
}