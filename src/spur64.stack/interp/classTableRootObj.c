/* Extracted from interp.c:33374 (function classTableRootObj). */

sqInt
classTableRootObj(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(hiddenRootsObj);
}