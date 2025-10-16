/* Extracted from interp.c:64226 (function stackFloatValue). */

double
stackFloatValue(sqInt offset)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return floatValueOf(longAt(GIV(stackPointer) + (offset * BytesPerWord)));
}