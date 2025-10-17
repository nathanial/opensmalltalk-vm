/* Extracted from interp.c:13423 (function primitiveClass). */

	/* InterpreterPrimitives>>#primitiveClass */

static void
primitiveClass(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt instance;
    sqInt oop;
    char *sp;
    sqInt tagBits;

	instance = longAt(stackPointer);
	if ((argumentCount > 0)
	 && (/* isOopForwarded: */
		((!(instance & (tagMask()))))
	 && ((!((longAt((void *)(instance))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
	else {
		oop = /* fetchClassOf: */
				((tagBits = instance & (tagMask()))
					? longAt((void *)((classTableFirstPage + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
					: fetchClassOfNonImm(instance));

		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),oop);
		stackPointer = sp;
	}
}