/* Extracted from interp.c:55145 (function isPrimitiveFunctionPointerAnIndex). */

static int
isPrimitiveFunctionPointerAnIndex(void)
{
	return (((usqIntptr_t) primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex;
}