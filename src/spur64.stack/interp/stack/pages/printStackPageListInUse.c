/* Extracted from interp.c:60916 (function printStackPageListInUse). */

/*	useful for VM debugging */

/* StackInterpreter>>#printStackPageListInUse */

void printStackPageListInUse(void) {
  sqInt n;
  StackPage *page;

  page = mostRecentlyUsedPage;
  n = 0;
  do {
    if (!(isFree(page))) {
      printStackPageuseCount(page, (n += 1));
      cr();
    }
  } while (((page = (page->prevPage))) != (mostRecentlyUsedPage));
}