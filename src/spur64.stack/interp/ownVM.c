/* Extracted from interp.c:58192 (function ownVM). */

/*	This is the entry-point for plugins and primitives that wish to reacquire
	the VM after having
	released it via disownVM or callbacks that want to acquire it without
	knowing their ownership
	status. While this exists for the threaded FFI VM we use it to reset
	newMethod and the
	argumentCount after a callback.
	
	Answer 0 if the current thread is known to the VM (and on return owns the
	VM). Answer 1 if the current thread is unknown to the VM and takes
	ownership. Answer -1 if the current thread is unknown to the VM and fails
	to take ownership. */

	/* StackInterpreter>>#ownVM: */

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