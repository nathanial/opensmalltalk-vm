/* Extracted from interp.c:21251 (function primitiveQuit). */

static void
primitiveQuit(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	ioExitWithErrorCode((GIV(argumentCount) == 1
			? ((longAt(GIV(stackPointer))) >> 3)
			: 0));
}