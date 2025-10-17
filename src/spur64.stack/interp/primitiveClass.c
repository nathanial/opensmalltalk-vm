/* Extracted from interp.c:13423 (function primitiveClass). */

	/* InterpreterPrimitives>>#primitiveClass */

static void
primitiveClass(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt instance;
    sqInt oop;
    char *sp;
    sqInt tagBits;

	instance = longAt(GIV(stackPointer));
	if ((GIV(argumentCount) > 0)
	 && (/* isOopForwarded: */
		((!(instance & (tagMask()))))
	 && ((!((longAt((void *)(instance))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
	else {
		oop = /* fetchClassOf: */
				((tagBits = instance & (tagMask()))
					? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
					: fetchClassOfNonImm(instance));

		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
		GIV(stackPointer) = sp;
	}
}