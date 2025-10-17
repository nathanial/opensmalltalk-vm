/* Extracted from interp.c:45705 (function statNumGCs). */

/*	Part of InterpreterProxy's 1.14 API */

	/* SpurMemoryManager>>#statNumGCs */

sqInt
statNumGCs(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (GIV(statScavenges) + GIV(statIncrGCs)) + GIV(statFullGCs);
}