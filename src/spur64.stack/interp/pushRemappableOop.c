/* Extracted from interp.c:44872 (function pushRemappableOop). */

/*	Record the given object in a the remap buffer. Objects in this buffer are
	remapped when a compaction occurs. This facility is used by the
	interpreter to ensure that
	objects in temporary variables are properly remapped.
	We support this excessence for compatibility with ObjectMemory.
	Spur doesn't GC during allocation. */

	/* SpurMemoryManager>>#pushRemappableOop: */

void
pushRemappableOop(sqInt oop)
{
	assert(addressCouldBeOop(oop));
	remapBuffer[(remapBufferCount += 1)] = oop;
	if (!(remapBufferCount <= RemapBufferSize)) {
		error("remapBuffer overflow");
	}
}