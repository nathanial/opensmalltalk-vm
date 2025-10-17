/* Extracted from interp.c:44094 (function printObjStackPagemyIndexpageTypeprintContents). */

	/* SpurMemoryManager>>#printObjStackPage:myIndex:pageType:printContents: */

static NoDbgRegParms void
printObjStackPagemyIndexpageTypeprintContents(sqInt objStackPage, sqInt myx, sqInt pageType, sqInt printContents)
{
    sqInt freeOrNextPage;
    sqInt index;
    int isFirstPage;
    int isFreePage;
    int isNextPage;
    sqInt page;

	isFirstPage = pageType == ObjStackMyx;
	isNextPage = pageType == ObjStackNextx;
	isFreePage = pageType == ObjStackFreex;
	printObjStackPagemyIndextag(objStackPage, myx, (isFirstPage
			? "head"
			: (isFreePage
					? "free"
					: "next")));
	if (isFirstPage
	 || (isNextPage)) {
		/* begin tab */
		printChar('\t');
		print("topx: ");
		printNum(longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord())))))));
		print(" next: ");
		printHex(longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord())))))));
		if (isFirstPage) {
			print(" free: ");
			printHex(longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord())))))));
		}
		cr();
	}
	if (isFirstPage) {
		freeOrNextPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))));
		while (freeOrNextPage != 0) {
			printObjStackPagemyIndexpageTypeprintContents(freeOrNextPage, myx, ObjStackFreex, 0);
			page = longAt((void *)((freeOrNextPage + BaseHeaderSize) + ((((usqInt)(ObjStackFreex) << (shiftForWord()))))));
			if ((page == freeOrNextPage)
			 || (page == objStackPage)) {
				print("circularity in free page list!!");
				cr();
				page = 0;
			}
			freeOrNextPage = page;
		}
	}
	freeOrNextPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
	if (freeOrNextPage) {
		printObjStackPagemyIndexpageTypeprintContents(freeOrNextPage, myx, ObjStackNextx, printContents);
	}
	if (printContents) {
		index = (longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) + ObjStackNextx;
		while (index >= ObjStackFixedSlots) {
			/* begin space */
			printChar(' ');
			printHex(longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord())))))));
			index -= 1;
		}
		if (((longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) + ObjStackNextx) >= ObjStackFixedSlots) {
			cr();
		}
	}
}