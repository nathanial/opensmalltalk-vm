/* Extracted from interp.c:19828 (function primitiveNoop). */

/*	A placeholder for primitives that haven't been implemented or are being
	withdrawn gradually. Just absorbs any arguments and returns the receiver.
 */

	/* InterpreterPrimitives>>#primitiveNoop */

static void
primitiveNoop(void)
{
	/* begin pop: */
	stackPointer += argumentCount * BytesPerWord;
}