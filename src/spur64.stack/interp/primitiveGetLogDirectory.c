/* Extracted from interp.c:17258 (function primitiveGetLogDirectory). */

EXPORT(sqInt)
primitiveGetLogDirectory(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    char *ptr;
    char *sp;
    sqInt stringOop;
    sqInt sz;

	ptr = ioGetLogDirectory();
	if (!ptr) {
		/* begin success: */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return null;
	}
	sz = strlen(ptr);
	stringOop = instantiateClassindexableSize(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassByteString) << (shiftForWord())))))), sz);
	for (i = 0; i < sz; i += 1) {
		/* storeByte:ofObject:withValue: */
		byteAtput((void *)((stringOop + BaseHeaderSize) + i),ptr[i]);
	}

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),stringOop);
	GIV(stackPointer) = sp;
	return 0;
}