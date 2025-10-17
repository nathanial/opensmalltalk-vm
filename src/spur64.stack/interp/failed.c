/* Extracted from interp.c:11134 (function failed). */

/*	In C, non-zero is true, so avoid computation by simply answering
	primFailCode in the C version.
 */

	/* InterpreterPrimitives>>#failed */

sqInt
failed(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return primFailCode;
}