/* Extracted from interp.c:65569 (function validInstructionPointerinFrame). */

static NoDbgRegParms sqInt
validInstructionPointerinFrame(usqInt anInstrPointer, char *fp)
{
	return /* validInstructionPointer:inMethod:framePointer: */
		(anInstrPointer >= ((((longAt(fp + FoxMethod)) + (lastPointerOf(longAt(fp + FoxMethod)))) + BytesPerOop) - 1))
	 && (anInstrPointer < ((((longAt(fp + FoxMethod)) + (numBytesOfBytes(longAt(fp + FoxMethod)))) + BaseHeaderSize) - 1));
}