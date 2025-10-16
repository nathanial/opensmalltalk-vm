/* Extracted from interp.c:54324 (function getImageHeaderFlags). */

static sqInt
getImageHeaderFlags(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((((((GIV(fullScreenFlag) + ((VMBIGENDIAN
		? 0
		: 2))) + ((GIV(preemptionYields)
		? 0
		: 16))) + ((GIV(newFinalization)
		? 64
		: 0))) + ((sendWheelEvents
		? 128
		: 0))) + ((primitiveDoMixedArithmetic
		? 0
		: 0x100))) + ((upscaleDisplayIfHighDPI
		? 0
		: 0x400))) + (((GIV(imageHeaderFlags) | (2003)) - (2003)));
}