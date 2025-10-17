/* Extracted from interp.c:37741 (function isValidObjStackPagemyIndexfirstPage).
 */

/*	Answer if the obj stack at stackRootIndex is valid. */

/* SpurMemoryManager>>#isValidObjStackPage:myIndex:firstPage: */

static sqInt isValidObjStackPagemyIndexfirstPage(sqInt objStackPage, sqInt myx,
                                                 sqInt isFirstPage) {
  sqInt freeOrNextPage;
  sqInt index;
  char *ns;
  sqInt page;

  if (!(isValidObjStackPagemyIndex(objStackPage, myx))) {
    return 0;
  }
  freeOrNextPage =
      fetchPointerofObject(ObjStackFreex, objStackPage);
  while (freeOrNextPage != 0) {
    if (!isFirstPage) {
      objStackInvalidBecause = "free page on other than first page";
      invalidObjStackPage = objStackPage;
      return 0;
    }
    if (freeOrNextPage ==
        (fetchPointerofObject(ObjStackNextx, objStackPage))) {
      objStackInvalidBecause = "free page = next page";
      invalidObjStackPage = freeOrNextPage;
      return 0;
    }
    if (!(isValidObjStackPagemyIndex(freeOrNextPage, myx))) {
      /* begin stretch:cat: */
      ns = malloc(((strlen(objStackInvalidBecause)) + (14)) + 2);
      strcpy(ns, objStackInvalidBecause);
      objStackInvalidBecause = ((char *)(strcat(ns, ", on next page")));
      return 0;
    }
    page = fetchPointerofObject(ObjStackFreex, freeOrNextPage);
    if ((page == freeOrNextPage) || (page == objStackPage)) {
      objStackInvalidBecause = "circularity in free page list";
      invalidObjStackPage = page;
      return 0;
    }
    freeOrNextPage = page;
  }
  if (isFirstPage) {
    if (!(((myx >= (1U << (22 /* classIndexFieldWidth */ -
                           (classTableMajorIndexShift())))) &&
           (myx <= (((1U << (22 /* classIndexFieldWidth */ -
                             (classTableMajorIndexShift()))) +
                     8 /* hiddenRootSlots */) -
                    1))))) {
      objStackInvalidBecause = "myx out of range";
      invalidObjStackPage = objStackPage;
      return 0;
    }
    if (!((fetchPointerofObject(myx, hiddenRootsObj)) ==
          objStackPage)) {
      objStackInvalidBecause = "firstPage is not root";
      invalidObjStackPage = objStackPage;
      return 0;
    }
  }
  index = fetchPointerofObject(ObjStackTopx, objStackPage);
  if (!(((index >= 0) && (index <= ObjStackLimit)))) {
    objStackInvalidBecause = "bad topx";
    invalidObjStackPage = objStackPage;
    return 0;
  }
  freeOrNextPage =
      fetchPointerofObject(ObjStackNextx, objStackPage);
  if (!freeOrNextPage) {
    return 1;
  }
  if (freeOrNextPage == objStackPage) {
    objStackInvalidBecause = "circularity in objStack page list";
    invalidObjStackPage = objStackPage;
    return 0;
  }
  return isValidObjStackPagemyIndexfirstPage(freeOrNextPage, myx, 0);
}