/* Extracted from interp.c:58296 (function popStack). */

sqInt
popStack(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt top;

	top = longAt(GIV(stackPointer));
	GIV(stackPointer) += BytesPerWord;
	return top;
}