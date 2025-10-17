/* Extracted from interp.c:21251 (function primitiveQuit). */

	/* InterpreterPrimitives>>#primitiveQuit */

static void
primitiveQuit(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	ioExitWithErrorCode((argumentCount == 1
			? ((longAt(stackPointer)) >> 3)
			: 0));
}