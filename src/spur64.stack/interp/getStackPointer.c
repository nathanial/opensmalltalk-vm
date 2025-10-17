/* Extracted from interp.c:54402 (function getStackPointer). */

/*	For Alien FFI */

	/* StackInterpreter>>#getStackPointer */

sqInt *
getStackPointer(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((sqInt *) stackPointer);
}