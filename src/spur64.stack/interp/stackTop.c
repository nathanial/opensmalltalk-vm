/* Extracted from interp.c:64459 (function stackTop). */

	/* StackInterpreter>>#stackTop */

sqInt
stackTop(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt(stackPointer);
}