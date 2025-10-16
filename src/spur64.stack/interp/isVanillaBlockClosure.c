/* Extracted from interp.c:55190 (function isVanillaBlockClosure). */

static NoDbgRegParms int
isVanillaBlockClosure(sqInt aClosure)
{
	assert((isPointers(aClosure))
	 && ((numSlotsOf(aClosure)) >= ClosureFirstCopiedValueIndex));
	return ((((longAt((void *)((aClosure + BaseHeaderSize) + ((((usqInt)(ClosureStartPCIndex) << (shiftForWord())))))))) & 7) == 1);
}