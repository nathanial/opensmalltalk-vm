/* Extracted from interp.c:35361 (function firstFixedFieldOfMaybeImmediate). */

static NoDbgRegParms void *
firstFixedFieldOfMaybeImmediate(sqInt oop)
{
	return (((oop & (tagMask())) != 0)
			? ((void *)oop)
			: pointerForOop(oop + BaseHeaderSize));
}