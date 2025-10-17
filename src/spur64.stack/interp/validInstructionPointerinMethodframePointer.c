/* Extracted from interp.c:65585 (function
 * validInstructionPointerinMethodframePointer). */

/*	Note that we accept anInstrPointer pointing to a callPrimitiveBytecode
        at the start of a method that contains a primitive. This because methods
        like Context(Part)>>reset have to be updated to skip the callPrimtiive
        bytecode otherwise. */
/*	-1 for pre-increment in fetchNextBytecode */

/* StackInterpreter>>#validInstructionPointer:inMethod:framePointer: */

sqInt validInstructionPointerinMethodframePointer(usqInt theInstrPointer,
                                                  usqInt aMethod, char *fp) {
  return (theInstrPointer >=
          (((aMethod + (lastPointerOf(aMethod))) + BytesPerOop) - 1)) &&
         (theInstrPointer <
          (((aMethod + (numBytesOfBytes(aMethod))) + BaseHeaderSize) - 1));
}