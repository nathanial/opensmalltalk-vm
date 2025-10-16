/* Extracted from interp.c:54385 (function getSnapshotScreenSize). */

static sqInt
getSnapshotScreenSize(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt screenSize;

	screenSize = ioScreenSize();
	if (((screenSize & 0xFFFF) == 0)
	 || ((((usqInt)(screenSize)) >> 16) == 0)) {
		screenSize = GIV(savedWindowSize);
	}
	return screenSize;
}