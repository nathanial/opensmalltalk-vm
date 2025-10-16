/* Extracted from interp.c:17826 (function primitiveImageFormatVersion). */

EXPORT(sqInt)
primitiveImageFormatVersion(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = ((((((usqInt)(/* imageFormatVersionForSnapshot */
	(GIV(multipleBytecodeSetsActive)
		? 68533 /* (imageFormatVersion bitOr: MultipleBytecodeSetsBitmask) */
		: 68021 /* imageFormatVersion */)))) & 0xFFFFFFFFU) << 3) | 1);

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),oop);
	GIV(stackPointer) = sp;
	return 0;
}