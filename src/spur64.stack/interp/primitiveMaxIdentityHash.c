/* Extracted from interp.c:19159 (function primitiveMaxIdentityHash). */

	/* InterpreterPrimitives>>#primitiveMaxIdentityHash */

static void
primitiveMaxIdentityHash(void)
{
    char *sp;

	/* begin pop:thenPushInteger: */
	longAtput((sp = stackPointer),(((usqInt)(identityHashHalfWordMask()) << 3) | 1));
	stackPointer = sp;
}