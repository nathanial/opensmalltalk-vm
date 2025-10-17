/* Extracted from interp.c:17258 (function primitiveGetLogDirectory). */

/* IMMUTABILITY */

/*	Primitive. Answer the VM's current log directory */

/* InterpreterPrimitives>>#primitiveGetLogDirectory */

EXPORT(sqInt)
primitiveGetLogDirectory(void) {
  sqInt i;
  char *ptr;
  char *sp;
  sqInt stringOop;
  sqInt sz;

  ptr = ioGetLogDirectory();
  if (!ptr) {
    /* begin success: */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return null;
  }
  sz = strlen(ptr);
  stringOop = instantiateClassindexableSize(
      fetchPointerofObject(ClassByteString, specialObjectsOop),
      sz);
  for (i = 0; i < sz; i += 1) {
    /* storeByte:ofObject:withValue: */
    byteAtput((void *)((stringOop + BaseHeaderSize) + i), ptr[i]);
  }

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            stringOop);
  stackPointer = sp;
  return 0;
}