/* Extracted from interp.c:44094 (function
 * printObjStackPagemyIndexpageTypeprintContents). */

/* SpurMemoryManager>>#printObjStackPage:myIndex:pageType:printContents: */

static void printObjStackPagemyIndexpageTypeprintContents(sqInt objStackPage,
                                                          sqInt myx,
                                                          sqInt pageType,
                                                          sqInt printContents) {
  sqInt freeOrNextPage;
  sqInt index;
  int isFirstPage;
  int isFreePage;
  int isNextPage;
  sqInt page;

  isFirstPage = pageType == ObjStackMyx;
  isNextPage = pageType == ObjStackNextx;
  isFreePage = pageType == ObjStackFreex;
  printObjStackPagemyIndextag(
      objStackPage, myx,
      (isFirstPage ? "head" : (isFreePage ? "free" : "next")));
  if (isFirstPage || (isNextPage)) {
    /* begin tab */
    printChar('\t');
    print("topx: ");
    printNum(
        fetchPointerofObject(ObjStackTopx, objStackPage));
    print(" next: ");
    printHex(
        fetchPointerofObject(ObjStackNextx, objStackPage));
    if (isFirstPage) {
      print(" free: ");
      printHex(
          fetchPointerofObject(ObjStackFreex, objStackPage));
    }
    cr();
  }
  if (isFirstPage) {
    freeOrNextPage =
        fetchPointerofObject(ObjStackFreex, objStackPage);
    while (freeOrNextPage != 0) {
      printObjStackPagemyIndexpageTypeprintContents(freeOrNextPage, myx,
                                                    ObjStackFreex, 0);
      page =
          fetchPointerofObject(ObjStackFreex, freeOrNextPage);
      if ((page == freeOrNextPage) || (page == objStackPage)) {
        print("circularity in free page list!!");
        cr();
        page = 0;
      }
      freeOrNextPage = page;
    }
  }
  freeOrNextPage =
      fetchPointerofObject(ObjStackNextx, objStackPage);
  if (freeOrNextPage) {
    printObjStackPagemyIndexpageTypeprintContents(freeOrNextPage, myx,
                                                  ObjStackNextx, printContents);
  }
  if (printContents) {
    index =
        (fetchPointerofObject(ObjStackTopx, objStackPage)) +
        ObjStackNextx;
    while (index >= ObjStackFixedSlots) {
      /* begin space */
      printChar(' ');
      printHex(fetchPointerofObject(index, objStackPage));
      index -= 1;
    }
    if (((fetchPointerofObject(ObjStackTopx, objStackPage)) +
         ObjStackNextx) >= ObjStackFixedSlots) {
      cr();
    }
  }
}