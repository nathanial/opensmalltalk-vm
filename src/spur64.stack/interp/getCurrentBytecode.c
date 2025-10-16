/* Extracted from interp.c:54173 (function getCurrentBytecode). */

sqInt
getCurrentBytecode(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return byteAt((void *)(GIV(instructionPointer)));
}