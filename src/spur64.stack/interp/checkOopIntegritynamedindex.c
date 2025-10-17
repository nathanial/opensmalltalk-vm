/* Extracted from interp.c:32961 (function checkOopIntegritynamedindex). */

	/* SpurMemoryManager>>#checkOopIntegrity:named:index: */

static NoDbgRegParms sqInt
checkOopIntegritynamedindex(sqInt obj, char *name, sqInt i)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if ((oopisLessThan(obj, GIV(endOfMemory)))
	 && ((heapMapAtWord(pointerForOop(obj))) != 0)) {
		return 1;
	}
	fprintf(GIV(transcript),
			"%s leak @ %d = %p\n",
			name,
			((int) i),
			((void *)obj));
	return 0;
}