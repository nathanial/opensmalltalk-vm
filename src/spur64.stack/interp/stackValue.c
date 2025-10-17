/* Extracted from interp.c:64469 (function stackValue). */

/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#stackValue: */

sqInt
stackValue(sqInt offset)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt(GIV(stackPointer) + (offset * BytesPerWord));
}