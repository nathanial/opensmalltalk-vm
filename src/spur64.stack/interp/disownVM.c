/* Extracted from interp.c:51758 (function disownVM). */

 /* SEND_PRINTING */


/*	Release the VM to other threads and answer the current thread's index.
	Currently valid flags for the non-threaded VM are:
	DisownVMForThreading	- allow the VM to thread-switch; this is ignored
	DisownVMForFFICall		- informs the VM that it is entering an FFI call
	
	This is the entry-point for plugins and primitives that wish to release
	the VM while
	performing some operation that may potentially block, and for callbacks
	returning back to some blocking operation. While this exists for the
	threaded FFI VM we use
	it to reset newMethod and the argumentCount after a callback. */

	/* StackInterpreter>>#disownVM: */

void *
disownVM(sqInt flags)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt top;
    void *vmHandle;

	assert(GIV(primFailCode) == 0);
	assert(flags != 0);

	/* In the single-threaded VM here, we treat flags directly as vmHandle. See MTVM. */
	vmHandle = ((void *) flags);

	/* We are either in disowning after a callback (see thunkEntry) or before an FFI
	   callout. */
	assert((flags == DisownVMFromCallback)
	 || ((flags == (flags & (DisownVMForFFICall + DisownVMForThreading)))
	 && (((flags & DisownVMForFFICall) != 0))));

	/* If DisownVMForFFICall this is from the FFI plugin and we're making a callout; remember the fact. */
	if (((flags & DisownVMForFFICall) != 0)) {
		assert((isOopCompiledMethod(GIV(newMethod)))
		 && ((argumentCountOf(GIV(newMethod))) == GIV(argumentCount)));
		return (GIV(ffiCalloutVMHandle) = vmHandle);
	}

	/* Otherwise this is a callback return; restore argumentCount and newMethod as per the ownVM: on callback. */
	if (((flags & DisownVMFromCallback) != 0)) {
		GIV(argumentCount) = ((popStack()) >> 3);
		assert(((GIV(argumentCount) >= 0) && (GIV(argumentCount) <= (argumentCountOfMethodHeader(-1)))));

		/* begin popStack */
		top = longAt(GIV(stackPointer));
		GIV(stackPointer) += BytesPerWord;
		GIV(newMethod) = top;
		assert((isOopCompiledMethod(GIV(newMethod)))
		 && ((argumentCountOf(GIV(newMethod))) == GIV(argumentCount)));
		return null;
	}
	return vmHandle;
}