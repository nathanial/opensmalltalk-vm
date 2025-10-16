/* Extracted from interp.c:19828 (function primitiveNoop). */

static void
primitiveNoop(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin pop: */
	GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
}