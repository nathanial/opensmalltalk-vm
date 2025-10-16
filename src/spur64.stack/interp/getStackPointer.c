/* Extracted from interp.c:54402 (function getStackPointer). */

sqInt *
getStackPointer(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((sqInt *) GIV(stackPointer));
}