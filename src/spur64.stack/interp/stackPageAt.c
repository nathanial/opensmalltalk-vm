/* Extracted from interp.c:11003 (function stackPageAt). */

static NoDbgRegParms StackPage *
stackPageAt(sqInt index)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return stackPageAtpages(index, GIV(pages));
}