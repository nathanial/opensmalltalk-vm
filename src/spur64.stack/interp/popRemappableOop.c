/* Extracted from interp.c:42439 (function popRemappableOop). */

/*	Pop and return the possibly remapped object from the remap buffer.
	We support this excessence for compatibility with ObjectMemory.
	Spur doesn't GC during allocation. */

	/* SpurMemoryManager>>#popRemappableOop */

sqInt
popRemappableOop(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;

	oop = GIV(remapBuffer)[GIV(remapBufferCount)];
	GIV(remapBufferCount) -= 1;
	return oop;
}