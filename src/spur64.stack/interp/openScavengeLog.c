/* Extracted from interp.c:28494 (function openScavengeLog). */

void
openScavengeLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	GIV(scavengeLog) = fopen("scavenge.log", "a+");
}