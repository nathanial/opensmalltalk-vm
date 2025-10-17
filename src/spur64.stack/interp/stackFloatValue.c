/* Extracted from interp.c:64226 (function stackFloatValue). */

/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#stackFloatValue: */

double
stackFloatValue(sqInt offset)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return floatValueOf(longAt(stackPointer + (offset * BytesPerWord)));
}