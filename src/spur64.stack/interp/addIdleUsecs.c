/* Extracted from interp.c:49900 (function addIdleUsecs). */

void
addIdleUsecs(sqInt idleUsecs)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	GIV(statIdleUsecs) += idleUsecs;
}