/* Extracted from interp.c:19828 (function primitiveNoop). */

/*	A placeholder for primitives that haven't been implemented or are being
	withdrawn gradually. Just absorbs any arguments and returns the receiver.
 */

	/* InterpreterPrimitives>>#primitiveNoop */

static void
primitiveNoop(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin pop: */
	GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
}