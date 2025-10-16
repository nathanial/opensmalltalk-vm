/* Extracted from interp.c:43607 (function printInvalidClassTableEntries). */

void
printInvalidClassTableEntries(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classOrNilSqInt;
    sqInt hash;
    sqInt i;
    sqInt index;
    sqInt j;
    sqInt page;
    sqInt toDoLimit;

	hash = 0;
	if (!(validClassTableRootPages())) {
		print("class table invalid; cannot print");
		cr();
		return;
	}

	/* begin classTableEntriesDo: */
	for (i = 0; i < GIV(numClassTablePages); i += 1) {
		page = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
		for (j = 0; j <= toDoLimit; j += 1) {
			classOrNilSqInt = longAt((void *)((page + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))));
			if (classOrNilSqInt != GIV(nilObj)) {
				index = ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
				if (((!((longAt((void *)(classOrNilSqInt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))
				 || ((((hash = (long32At((void *)(classOrNilSqInt + 4))) & (identityHashHalfWordMask()))) == 0)
				 || ((noCheckClassAtIndex(hash)) != classOrNilSqInt))) {
					print("entry ");
					printHex(index);
					print(" oop ");
					printHex(classOrNilSqInt);
					print(" hash ");
					printHex(hash);
					print(" => ");
					printHex(classAtIndex(hash));
					cr();
				}
			}
		}
	}
}