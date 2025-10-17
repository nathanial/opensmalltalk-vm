/* Extracted from interp.c:22563 (function primitiveSignal). */

/*	Synchronously signal the semaphore.
	This may change the active process as a result. */

	/* InterpreterPrimitives>>#primitiveSignal */

static void
primitiveSignal(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	synchronousSignal(longAt(GIV(stackPointer)));
}