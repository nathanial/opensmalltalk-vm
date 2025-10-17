/* Extracted from interp.c:63032 (function setSavedWindowSize). */

	/* StackInterpreter>>#setSavedWindowSize: */

void
setSavedWindowSize(sqInt value)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	savedWindowSize = value;
}