/* Extracted from interp.c:25780 (function primitiveVMPath). */

/*	Return a string containing the path name of VM's directory. */

	/* InterpreterPrimitives>>#primitiveVMPath */

static void
primitiveVMPath(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt s;
    char *sp;
    sqInt sz;

	sz = vmPathSize();
	s = instantiateClassindexableSize(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassByteString) << (shiftForWord())))))), sz);
	vmPathGetLength(s + BaseHeaderSize, sz);

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),s);
	GIV(stackPointer) = sp;
}