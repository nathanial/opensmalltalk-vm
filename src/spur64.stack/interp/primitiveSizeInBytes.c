/* Extracted from interp.c:22733 (function primitiveSizeInBytes). */

	/* InterpreterPrimitives>>#primitiveSizeInBytes */

static void
primitiveSizeInBytes(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqLong byteSize;
    sqInt oop;
    char *sp;

	byteSize = /* totalByteSizeOf: */
			((((longAt(GIV(stackPointer))) & (tagMask())) != 0)
				? 0
				: bytesInBody(longAt(GIV(stackPointer))));
	oop = positive64BitIntegerFor(byteSize);

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
	GIV(stackPointer) = sp;
}