/* Extracted from interp.c:58310 (function pop). */

/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#pop: */

sqInt
pop(sqInt nItems)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	stackPointer += nItems * BytesPerWord;
	return null;
}