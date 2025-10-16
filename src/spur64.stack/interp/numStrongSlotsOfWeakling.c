/* Extracted from interp.c:41328 (function numStrongSlotsOfWeakling). */

static NoDbgRegParms sqInt
numStrongSlotsOfWeakling(sqInt objOop)
{
    sqInt objOopSqInt;

	assert((formatOf(objOop)) == (weakArrayFormat()));
	objOopSqInt = fetchClassOfNonImm(objOop);

	/* begin fixedFieldsOfClass: */
	return (((longAt((void *)((objOopSqInt + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)) & ((1U << (fixedFieldsFieldWidth())) - 1);
}