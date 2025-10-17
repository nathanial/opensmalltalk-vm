/* Extracted from interp.c:15136 (function primitiveExitToDebugger). */

	/* InterpreterPrimitives>>#primitiveExitToDebugger */

static void
primitiveExitToDebugger(void)
{
	error("Exit to debugger at user request");
}