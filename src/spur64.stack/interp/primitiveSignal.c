/* Extracted from interp.c:22563 (function primitiveSignal). */

static void
primitiveSignal(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	synchronousSignal(longAt(GIV(stackPointer)));
}