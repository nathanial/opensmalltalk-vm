/* Extracted from interp.c:65014 (function superclassOf). */

sqInt
superclassOf(sqInt classPointer)
{
    sqInt objOop;

	/* begin followObjField:ofObject: */
	objOop = longAt((void *)((classPointer + BaseHeaderSize) + ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
	assert(isNonImmediate(objOop));
	if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex, classPointer, objOop);
	}
	return objOop;
}