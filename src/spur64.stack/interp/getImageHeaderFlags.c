/* Extracted from interp.c:54324 (function getImageHeaderFlags). */

/*	Answer the flags that are contained in the 7th long of the image header. */

	/* StackInterpreter>>#getImageHeaderFlags */

static sqInt
getImageHeaderFlags(void)
{
	return ((((((fullScreenFlag + ((VMBIGENDIAN
		? 0
		: 2))) + ((preemptionYields
		? 0
		: 16))) + ((newFinalization
		? 64
		: 0))) + ((sendWheelEvents
		? 128
		: 0))) + ((primitiveDoMixedArithmetic
		? 0
		: 0x100))) + ((upscaleDisplayIfHighDPI
		? 0
		: 0x400))) + (((imageHeaderFlags | (2003)) - (2003)));
}