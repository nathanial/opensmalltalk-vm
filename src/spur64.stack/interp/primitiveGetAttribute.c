/* Extracted from interp.c:17102 (function primitiveGetAttribute). */

/*	Fetch the system attribute with the given integer ID. The result is a
	string, or nil if the attribute is not defined.
 */

	/* InterpreterPrimitives>>#primitiveGetAttribute */

static void
primitiveGetAttribute(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt attr;
    const char *attributeString;
    char *sp;

	attr = longAt(stackPointer);
	if (!((((attr) & 7) == 1))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	if ((attributeString = getAttributeString((attr >> 3)))) {
		methodReturnString(attributeString);
	}
	else {
		/* begin methodReturnValue: */
		assert(!((failed())));
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),nilObj);
		stackPointer = sp;
	}
}