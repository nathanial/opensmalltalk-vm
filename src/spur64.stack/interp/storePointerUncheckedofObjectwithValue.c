/* Extracted from interp.c:46053 (function storePointerUncheckedofObjectwithValue). */

sqInt
storePointerUncheckedofObjectwithValue(sqInt fieldIndex, sqInt objOop, sqInt valuePointer)
{
	assert((isNonImmediate(objOop))
	 && (!(isForwarded(objOop))));
	assert(validStorePointerUncheckedArgs(fieldIndex, objOop, valuePointer));
	return longAtput((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),valuePointer);
}