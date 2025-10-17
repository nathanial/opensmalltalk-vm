/* Extracted from interp.c:23174 (function primitiveSmallFloatExp). */

/*	Computes E raised to the receiver power.
	Since SmallFloats cannot represent NaNs there's no need to special case. */

	/* InterpreterPrimitives>>#primitiveSmallFloatExp */

static void
primitiveSmallFloatExp(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aValue;

	aValue = floatObjectOf(exp(smallFloatValueOf(longAt(stackPointer))));

	/* begin stackTopPut: */
	longAtput(stackPointer,aValue);
}