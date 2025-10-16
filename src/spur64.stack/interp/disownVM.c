/* Extracted from interp.c:51758 (function disownVM). */

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