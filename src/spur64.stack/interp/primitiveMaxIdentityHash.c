/* Extracted from interp.c:19159 (function primitiveMaxIdentityHash). */

	/* InterpreterPrimitives>>#primitiveMaxIdentityHash */

static void
primitiveMaxIdentityHash(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer)),(((usqInt)(identityHashHalfWordMask()) << 3) | 1));
	GIV(stackPointer) = sp;
}