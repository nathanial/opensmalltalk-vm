/* Extracted from interp.c:46680 (function validClassTableHashes). */

/*	Check the hashes of classes in the table. The tricky thing here is that
	classes may be duplicated
	in the table. So each entry must be in the table at its hash, even if it
	is elsewhere in the table. */

	/* SpurMemoryManager>>#validClassTableHashes */

static sqInt
validClassTableHashes(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classOrNilSqInt;
    sqInt hash;
    sqInt i;
    sqInt ignored;
    sqInt j;
    sqInt page;
    sqInt toDoLimit;

	if (!(validClassTableRootPages())) {
		return 0;
	}

	/* begin classTableEntriesDo: */
	for (i = 0; i < GIV(numClassTablePages); i += 1) {
		page = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
		for (j = 0; j <= toDoLimit; j += 1) {
			classOrNilSqInt = longAt((void *)((page + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))));
			if (classOrNilSqInt != GIV(nilObj)) {
				ignored = ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
				if ((!((longAt((void *)(classOrNilSqInt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					return 0;
				}
				hash = (long32At((void *)(classOrNilSqInt + 4))) & (identityHashHalfWordMask());
				if (!hash) {
					return 0;
				}
				if ((noCheckClassAtIndex(hash)) != classOrNilSqInt) {
					return 0;
				}
			}
		}
	}
	return 1;
}