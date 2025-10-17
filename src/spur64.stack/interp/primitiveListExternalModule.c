/* Extracted from interp.c:18872 (function primitiveListExternalModule). */

/*	Primitive. Answer the n-th loaded external module name. */

	/* InterpreterPrimitives>>#primitiveListExternalModule */

static void
primitiveListExternalModule(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt index;
    sqInt integerPointer;
    char *moduleName;
    char *sp;

	if (!(GIV(argumentCount) == 1)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		index = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		index = 0;
	}
	if (GIV(primFailCode)
	 || (index <= 0)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}
	if ((moduleName = ioListLoadedModule(index))) {
		methodReturnString(moduleName);
	}
	else {
		/* begin methodReturnValue: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),GIV(nilObj));
		GIV(stackPointer) = sp;
	}
}