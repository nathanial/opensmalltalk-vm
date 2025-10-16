/* Extracted from interp.c:36996 (function instanceSizeOf). */

sqInt
instanceSizeOf(sqInt classObj)
{
	assert(addressCouldBeClassObj(classObj));
	return (((longAt((void *)((classObj + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)) & ((1U << (fixedFieldsFieldWidth())) - 1);
}