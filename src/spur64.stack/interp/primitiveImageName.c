/* Extracted from interp.c:17851 (function primitiveImageName). */

/*	When called with a single string argument, record the string as the
	current image file name.
	When called with zero arguments, return a string containing the current
	image file name.
 */

	/* InterpreterPrimitives>>#primitiveImageName */

static void
primitiveImageName(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ccIndex;
    sqInt isString;
    sqInt okToRename;
    sqInt s;
    void *sCRIfn;
    char *sp;
    sqInt sz;

	if (argumentCount == 1) {
		s = longAt(stackPointer);

		/* begin isInstanceOfClassByteString: */
		/* begin is:instanceOf:compactClassIndex: */
		if (((s & (tagMask())) != 0)) {
			isString = 0;
			goto l1;
		}

		/* begin isClassOfNonImm:equalTo:compactClassIndex: */
		assert(!(isImmediate(s)));
		ccIndex = (longAt((void *)(s))) & (classIndexMask());
		isString = ClassByteStringCompactIndex == ccIndex;
		/* end isInstanceOfClassByteString: */
l1:
		if (!isString) {
			/* primitiveFailFor: */
			primFailCode = PrimErrBadArgument;
			return;
		}

		/* If the security plugin can be loaded, use it to check for rename permission.
		   If not, assume it's ok */
		sCRIfn = ioLoadFunctionFrom("secCanRenameImage", "SecurityPlugin");
		if (sCRIfn) {
			okToRename = ((sqInt (*)(void))sCRIfn)();
			if (!okToRename) {
				/* primitiveFailFor: */
				primFailCode = PrimErrUnsupported;
				return;
			}
		}
		imageNamePutLength(s + BaseHeaderSize, numBytesOf(s));

		/* begin pop: */
		stackPointer += 1 * BytesPerWord;
		return;
	}
	sz = imageNameSize();
	s = instantiateClassindexableSize(longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassByteString) << (shiftForWord())))))), sz);
	imageNameGetLength(s + BaseHeaderSize, sz);

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),s);
	stackPointer = sp;
}