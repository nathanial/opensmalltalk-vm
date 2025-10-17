/* Extracted from interp.c:60970 (function printStackPageuseCount). */

/* StackInterpreter>>#printStackPage:useCount: */

static NoDbgRegParms void printStackPageuseCount(StackPage *page, sqInt n) {
  print("page ");
  printHexPtrnp(page);
  print(" (");
  printNum(pageIndexForstackMemorybytesPerPage((page->realStackLimit),
                                               stackMemory, bytesPerPage));
  if (n >= 0) {
    print(",");
    printNum(n);
  }
  print(")  (trace: ");
  printNum((page->trace));
  printChar(')');
  if (isFree(page)) {
    print(" (free)");
  }
  if (page == (mostRecentlyUsedPage)) {
    print(" (MRU)");
  }
  if (((page->prevPage)) == (mostRecentlyUsedPage)) {
    print(" (LRU)");
  }
  cr();

  /* begin tab */
  printChar('\t');
  print("ba: ");
  printHexPtrnp((page->baseAddress));
  print(" - sl: ");
  printHexPtrnp((page->realStackLimit));
  print(" - sl-so: ");
  printHexPtrnp(((page->realStackLimit)) -
                (((FrameSlots + LargeContextSlots) + 1) * BytesPerWord));
  print(" - la:");
  printHexPtrnp((page->lastAddress));
  if (!(isFree(page))) {
    cr();

    /* begin tab */
    printChar('\t');
    print("baseFP ");
    printHexPtrnp((page->baseFP));

    /* cr; */

    /* begin tab */
    printChar('\t');
    print("headFP ");
    printHexPtrnp((page->headFP));

    /* cr; */

    /* begin tab */
    printChar('\t');
    print("headSP ");
    printHexPtrnp((page->headSP));
  }
  cr();

  /* begin tab */
  printChar('\t');
  print("prev ");
  printHexPtrnp(page->prevPage);
  print(" (");
  printNum(pageIndexForstackMemorybytesPerPage(
      (((page->prevPage))->realStackLimit), stackMemory, bytesPerPage));
  printChar(')');

  /* begin tab */
  printChar('\t');
  print("next ");
  printHexPtrnp(page->nextPage);
  print(" (");
  printNum(pageIndexForstackMemorybytesPerPage(
      (((page->nextPage))->realStackLimit), stackMemory, bytesPerPage));
  printChar(')');
  cr();
}