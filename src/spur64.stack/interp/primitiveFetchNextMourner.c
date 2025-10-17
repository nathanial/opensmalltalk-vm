/* Extracted from interp.c:15401 (function primitiveFetchNextMourner). */

	/* InterpreterPrimitives>>#primitiveFetchNextMourner */

static void
primitiveFetchNextMourner(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt mourner;
    char *sp;

	mourner = /* dequeueMourner */
			(mournQueue != nilObj
				? (assert(isValidObjStack(mournQueue)),
				popObjStack(mournQueue))
				: 0);
	if (mourner) {
		assert(isValidObjStack(mournQueue));

		/* begin pop:thenPush: */
		longAtput((sp = stackPointer),mourner);
		stackPointer = sp;
	}
	else {
		/* primitiveFailFor: */
		primFailCode = PrimErrNotFound;
	}
}