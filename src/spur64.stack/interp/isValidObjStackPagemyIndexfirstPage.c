/* Extracted from interp.c:37741 (function isValidObjStackPagemyIndexfirstPage). */

static NoDbgRegParms sqInt
isValidObjStackPagemyIndexfirstPage(sqInt objStackPage, sqInt myx, sqInt isFirstPage)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt freeOrNextPage;
    sqInt index;
    char *ns;
    sqInt page;

	if (!(isValidObjStackPagemyIndex(objStackPage, myx))) {
		return 0;
	}
	freeOrNextPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))));
	while (freeOrNextPage != 0) {
		if (!isFirstPage) {
			GIV(objStackInvalidBecause) = "free page on other than first page";
			GIV(invalidObjStackPage) = objStackPage;
			return 0;
		}
		if (freeOrNextPage == (longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord())))))))) {
			GIV(objStackInvalidBecause) = "free page = next page";
			GIV(invalidObjStackPage) = freeOrNextPage;
			return 0;
		}
		if (!(isValidObjStackPagemyIndex(freeOrNextPage, myx))) {
			/* begin stretch:cat: */
			ns = malloc(((strlen(GIV(objStackInvalidBecause))) + (14)) + 2);
			strcpy(ns, GIV(objStackInvalidBecause));
			GIV(objStackInvalidBecause) = ((char *) (strcat(ns, ", on next page")));
			return 0;
		}
		page = longAt((void *)((freeOrNextPage + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))));
		if ((page == freeOrNextPage)
		 || (page == objStackPage)) {
			GIV(objStackInvalidBecause) = "circularity in free page list";
			GIV(invalidObjStackPage) = page;
			return 0;
		}
		freeOrNextPage = page;
	}
	if (isFirstPage) {
		if (!(((myx >= (1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift())))) && (myx <= (((1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift()))) + 8 /* hiddenRootSlots */) - 1))))) {
			GIV(objStackInvalidBecause) = "myx out of range";
			GIV(invalidObjStackPage) = objStackPage;
			return 0;
		}
		if (!((longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(myx) << (shiftForWord()))))))) == objStackPage)) {
			GIV(objStackInvalidBecause) = "firstPage is not root";
			GIV(invalidObjStackPage) = objStackPage;
			return 0;
		}
	}
	index = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
	if (!(((index >= 0) && (index <= ObjStackLimit)))) {
		GIV(objStackInvalidBecause) = "bad topx";
		GIV(invalidObjStackPage) = objStackPage;
		return 0;
	}
	freeOrNextPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
	if (!freeOrNextPage) {
		return 1;
	}
	if (freeOrNextPage == objStackPage) {
		GIV(objStackInvalidBecause) = "circularity in objStack page list";
		GIV(invalidObjStackPage) = objStackPage;
		return 0;
	}
	return isValidObjStackPagemyIndexfirstPage(freeOrNextPage, myx, 0);
}