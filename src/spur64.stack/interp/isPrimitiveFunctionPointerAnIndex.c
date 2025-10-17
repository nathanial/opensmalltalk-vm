/* Extracted from interp.c:55145 (function isPrimitiveFunctionPointerAnIndex). */

/*	We save slots in the method cache by using the primitiveFunctionPointer
	to hold either a function pointer or the index of a quick primitive. Since
	quick primitive indices are small they can't be confused with function
	addresses. */

	/* StackInterpreter>>#isPrimitiveFunctionPointerAnIndex */

static int
isPrimitiveFunctionPointerAnIndex(void)
{
	return (((usqIntptr_t) primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex;
}