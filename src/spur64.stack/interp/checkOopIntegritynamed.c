/* Extracted from interp.c:32946 (function checkOopIntegritynamed). */

	/* SpurMemoryManager>>#checkOopIntegrity:named: */

static NoDbgRegParms sqInt
checkOopIntegritynamed(sqInt obj, char *name)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if ((oopisLessThan(obj, endOfMemory))
	 && ((heapMapAtWord(pointerForOop(obj))) != 0)) {
		return 1;
	}
	fprintf(transcript,
			"%s leak %p\n",
			name,
			((void *)obj));
	return 0;
}