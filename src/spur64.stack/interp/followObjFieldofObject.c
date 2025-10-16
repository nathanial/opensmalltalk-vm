/* Extracted from interp.c:35696 (function followObjFieldofObject). */

static NoDbgRegParms sqInt
followObjFieldofObject(sqInt fieldIndex, sqInt anObject)
{
    sqInt objOop;

	objOop = longAt((void *)((anObject + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	assert(isNonImmediate(objOop));
	if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		objOop = fixFollowedFieldofObjectwithInitialValue(fieldIndex, anObject, objOop);
	}
	return objOop;
}