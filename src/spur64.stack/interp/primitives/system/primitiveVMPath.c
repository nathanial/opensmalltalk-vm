/* Extracted from interp.c:25780 (function primitiveVMPath). */

/*	Return a string containing the path name of VM's directory. */

/* InterpreterPrimitives>>#primitiveVMPath */

static void primitiveVMPath(void) {
  sqInt s;
  sqInt sz;

  sz = vmPathSize();
  s = instantiateClassindexableSize(
      fetchPointerofObject(ClassByteString, specialObjectsOop), sz);
  vmPathGetLength(s + BaseHeaderSize, sz);

  popthenPush(1, s);
}
