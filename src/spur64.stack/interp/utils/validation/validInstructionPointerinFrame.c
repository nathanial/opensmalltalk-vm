/* Extracted from interp.c:65569 (function validInstructionPointerinFrame). */

/*	Note that we accept anInstrPointer pointing to a callPrimitiveBytecode
        at the start of a method that contains a primitive. This because methods
        like Context(Part)>>reset have to be updated to skip the callPrimtiive
        bytecode otherwise. */

/* StackInterpreter>>#validInstructionPointer:inFrame: */

static sqInt validInstructionPointerinFrame(usqInt anInstrPointer, char *fp) {
  return /* validInstructionPointer:inMethod:framePointer: */
      (anInstrPointer >=
       ((((longAt(fp + FoxMethod)) + (lastPointerOf(longAt(fp + FoxMethod)))) +
         BytesPerOop) -
        1)) &&
      (anInstrPointer < ((((longAt(fp + FoxMethod)) +
                           (numBytesOfBytes(longAt(fp + FoxMethod)))) +
                          BaseHeaderSize) -
                         1));
}