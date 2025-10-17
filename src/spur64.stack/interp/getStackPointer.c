/* Extracted from interp.c:54402 (function getStackPointer). */

/*	For Alien FFI */

	/* StackInterpreter>>#getStackPointer */

sqInt *
getStackPointer(void)
{
	return ((sqInt *) stackPointer);
}