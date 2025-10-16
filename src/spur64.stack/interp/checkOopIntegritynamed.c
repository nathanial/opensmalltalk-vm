/* Extracted from interp.c:32946 (function checkOopIntegritynamed). */

static NoDbgRegParms sqInt
checkOopIntegritynamed(sqInt obj, char *name)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if ((oopisLessThan(obj, GIV(endOfMemory)))
	 && ((heapMapAtWord(pointerForOop(obj))) != 0)) {
		return 1;
	}
	fprintf(GIV(transcript),
			"%s leak %p\n",
			name,
			((void *)obj));
	return 0;
}