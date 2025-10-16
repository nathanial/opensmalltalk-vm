/* Extracted from interp.c:17102 (function primitiveGetAttribute). */

static void
primitiveGetAttribute(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt attr;
    const char *attributeString;
    char *sp;

	attr = longAt(GIV(stackPointer));
	if (!((((attr) & 7) == 1))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	if ((attributeString = getAttributeString((attr >> 3)))) {
		methodReturnString(attributeString);
	}
	else {
		/* begin methodReturnValue: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),GIV(nilObj));
		GIV(stackPointer) = sp;
	}
}