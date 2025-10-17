/* Extracted from interp.c:11003 (function stackPageAt). */

/*	Answer the page for a page index.
	N.B. This is a zero-relative index. */

	/* CogStackPages>>#stackPageAt: */

static NoDbgRegParms StackPage *
stackPageAt(sqInt index)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return stackPageAtpages(index, GIV(pages));
}