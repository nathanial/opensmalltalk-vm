/* Extracted from interp.c:65585 (function validInstructionPointerinMethodframePointer). */

sqInt
validInstructionPointerinMethodframePointer(usqInt theInstrPointer, usqInt aMethod, char *fp)
{
	return (theInstrPointer >= (((aMethod + (lastPointerOf(aMethod))) + BytesPerOop) - 1))
	 && (theInstrPointer < (((aMethod + (numBytesOfBytes(aMethod))) + BaseHeaderSize) - 1));
}