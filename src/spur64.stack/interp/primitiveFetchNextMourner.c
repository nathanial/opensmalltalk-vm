/* Extracted from interp.c:15401 (function primitiveFetchNextMourner). */

	/* InterpreterPrimitives>>#primitiveFetchNextMourner */

static void
primitiveFetchNextMourner(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt mourner;
    char *sp;

	mourner = /* dequeueMourner */
			(GIV(mournQueue) != GIV(nilObj)
				? (assert(isValidObjStack(GIV(mournQueue))),
				popObjStack(GIV(mournQueue)))
				: 0);
	if (mourner) {
		assert(isValidObjStack(GIV(mournQueue)));

		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer)),mourner);
		GIV(stackPointer) = sp;
	}
	else {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrNotFound;
	}
}