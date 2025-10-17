/* Extracted from interp.c:46790 (function validFreeTreeChunkparent). */

	/* SpurMemoryManager>>#validFreeTreeChunk:parent: */

static NoDbgRegParms const char *
validFreeTreeChunkparent(sqInt chunk, sqInt parent)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    const char *reason;

	if (!chunk) {
		return null;
	}
	if (!(/* addressCouldBeOldObj: */
			((chunk & (BaseHeaderSize - 1)) == 0)
		 && (oopisGreaterThanOrEqualToandLessThan(chunk, oldSpaceStart, endOfMemory)))) {
		return "not in old space";
	}
	if (((bytesInBody(chunk)) / 8 /* allocationUnit */) < 64 /* numFreeLists */) {
		return "too small";
	}
	if (parent != (longAt((void *)((chunk + BaseHeaderSize) + (2U << (shiftForWord())))))) {
		return "bad parent";
	}
	if ((segmentContainingObj(chunk)) != (segmentContainingObj(addressAfter(chunk)))) {
		return "not in one segment";
	}
	if ((reason = validFreeTreeChunkparent(longAt((void *)((chunk + BaseHeaderSize) + (3U << (shiftForWord())))), chunk))) {
		return reason;
	}
	if ((reason = validFreeTreeChunkparent(longAt((void *)((chunk + BaseHeaderSize) + (4U << (shiftForWord())))), chunk))) {
		return reason;
	}
	return null;
}