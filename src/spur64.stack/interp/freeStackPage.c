/* Extracted from interp.c:10853 (function freeStackPage). */

/*	<InterpreterStackPage> */
/*	MRUP-->used page<->used page<->used page<->used page<--LRUP
        ^ <-next-prev-> ^
        | |
        v <-prev-next-> v
        free page<->free page<->free page<->free page */

/* CogStackPages>>#freeStackPage: */

static void freeStackPage(StackPage *aPage) {
  freeStackPageNoAssert(aPage);
  assert(pageListIsWellFormed());
}