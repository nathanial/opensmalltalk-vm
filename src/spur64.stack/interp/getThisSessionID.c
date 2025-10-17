/* Extracted from interp.c:54412 (function getThisSessionID). */

/*	return the global session ID value */

	/* StackInterpreter>>#getThisSessionID */

sqInt
getThisSessionID(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return globalSessionID;
}