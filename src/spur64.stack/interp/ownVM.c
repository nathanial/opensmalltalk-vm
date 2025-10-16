/* Extracted from interp.c:58192 (function ownVM). */

sqInt
ownVM(void *vmHandle)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt flags;
    char *sp;

	assert((isOopCompiledMethod(GIV(newMethod)))
	 && ((argumentCountOf(GIV(newMethod))) == GIV(argumentCount)));

	/* From callbacks (see thunkEntry), we don't know our ownerhip. Stash newMethod
	   and argumentCount on the stack. */
	if (!vmHandle) {
		assert(GIV(primFailCode) == 0);

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(newMethod));
		GIV(stackPointer) = sp;

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),(((usqInt)GIV(argumentCount) << 3) | 1));
		GIV(stackPointer) = sp;
		return 1;
	}
	flags = ((sqInt) vmHandle);
	if (((flags & DisownVMForFFICall) != 0)) {
		GIV(ffiCalloutVMHandle) = null;
	}
	return 0;
}