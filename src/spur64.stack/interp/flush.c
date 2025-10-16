/* Extracted from interp.c:53609 (function flush). */

void
flush(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	fflush(GIV(transcript));
}