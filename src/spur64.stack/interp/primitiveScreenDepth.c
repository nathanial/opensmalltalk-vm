/* Extracted from interp.c:21789 (function primitiveScreenDepth). */

/*	Return a SmallInteger indicating the current depth of the OS screen.
	Negative values are used to imply LSB type pixel format an there is some
	support in the VM for handling either MSB or LSB
 */

	/* InterpreterPrimitives>>#primitiveScreenDepth */

EXPORT(sqInt)
primitiveScreenDepth(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt depth;
    char *sp;

	depth = ioScreenDepth();
	if (GIV(primFailCode)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return null;
	}

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer)),(((usqInt)depth << 3) | 1));
	GIV(stackPointer) = sp;
	return 0;
}