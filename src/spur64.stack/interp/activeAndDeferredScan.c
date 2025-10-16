/* Extracted from interp.c:29627 (function activeAndDeferredScan). */

static NoDbgRegParms NeverInline sqInt
activeAndDeferredScan(sqInt anEphemeron)
{
    sqInt key;

	key = 0;
	assert(isEphemeron(anEphemeron));
	if ((isImmediate((/* begin keyOfEphemeron: */
		assert((isNonImmediate(anEphemeron))
		 && (isObjEphemeron(anEphemeron))),
	(key = longAt((void *)((anEphemeron + BaseHeaderSize) + (0U << (shiftForWord()))))))))
	 || (((byteAt((void *)(key + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)) {
		return 0;
	}
	return pushOnUnscannedEphemeronsStack(anEphemeron);
}