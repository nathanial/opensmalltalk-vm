/* Extracted from interp.c:64469 (function stackValue). */

sqInt
stackValue(sqInt offset)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return longAt(GIV(stackPointer) + (offset * BytesPerWord));
}