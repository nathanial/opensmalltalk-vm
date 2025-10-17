/* Extracted from interp.c:13457 (function primitiveClearVMProfile). */

/*	Primitive. Void the VM profile histograms. */

	/* InterpreterPrimitives>>#primitiveClearVMProfile */

static void
primitiveClearVMProfile(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	ioClearProfile();

	/* begin pop: */
	stackPointer += argumentCount * BytesPerWord;
}