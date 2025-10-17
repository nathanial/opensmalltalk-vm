/* Extracted from interp.c:54173 (function getCurrentBytecode). */

/*	currentBytecode will be private to the main dispatch loop in the generated
	code. This method allows the currentBytecode to be retrieved from global
	variables. 
 */

	/* StackInterpreter>>#getCurrentBytecode */

sqInt
getCurrentBytecode(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return byteAt((void *)(GIV(instructionPointer)));
}