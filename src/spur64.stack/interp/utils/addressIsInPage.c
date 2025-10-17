/* Extracted from interp.c:10797 (function addressIsInPage). */

/*	For assert-checking */

/* CogStackPage>>#addressIsInPage: */

static sqInt addressIsInPage(StackPage *self_in_CogStackPage,
                                           char *address) {
  return (((self_in_CogStackPage->lastAddress)) < address) &&
         (address < ((self_in_CogStackPage->baseAddress)));
}