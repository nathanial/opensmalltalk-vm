/* Extracted from interp.c:23174 (function primitiveSmallFloatExp). */

static void
primitiveSmallFloatExp(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aValue;

	aValue = floatObjectOf(exp(smallFloatValueOf(longAt(GIV(stackPointer)))));

	/* begin stackTopPut: */
	longAtput(GIV(stackPointer),aValue);
}