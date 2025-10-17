/* Extracted from interp.c:36204 (function headerForSlotshashformatclassIndex). */

/*	The header format in LSB is
	MSB:	| 8: numSlots		| (on a byte boundary)
	| 2 bits				|	(msb,lsb = {isMarked,isGrey})
	| 22: identityHash	| (on a word boundary)
	| 3 bits				|	(msb <-> lsb = {UNUSED,isPinned,isRemembered}
	| 5: format			| (on a byte boundary)
	| 2 bits				|	(msb,lsb = {isImmutable,isExtraAligned})
	| 22: classIndex		| (on a word boundary) : LSB
	The 7 bit flags are used (or reserved) for
	isExtraAligned		(bit 22)
	isImmutable		(bit 23)
	isRemembered	(bit 29)
	isPinned			(bit 30)
	UNUSED			(bit 31)
	isGrey				(bit 54)
	isMarked			(bit 55)
	The three bit field { UNUSED, isPinned, isRemembered } is for bits that
	are never
	set in young objects (forcing uses of the UNUSED bit to be for oldSPace
	objects only).
	This allows the remembered table to be pruned when full by using these
	bits as a
	reference count of newSpace objects from the remembered table. Objects
	with a
	high count should be tenured to prune the remembered table. */

	/* SpurMemoryManager>>#headerForSlots:hash:format:classIndex: */

static NoDbgRegParms usqLong
headerForSlotshashformatclassIndex(sqInt numSlots, sqInt hash, sqInt formatField, sqInt classIndex)
{
	return (((((((usqLong) numSlots)) << (numSlotsFullShift()))) + (((((usqLong) hash)) << (identityHashFullWordShift())))) + ((((usqInt)(formatField) << (formatShift()))))) + classIndex;
}