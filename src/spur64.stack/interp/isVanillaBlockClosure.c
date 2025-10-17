/* Extracted from interp.c:55190 (function isVanillaBlockClosure). */

/*	Answer if aClosure is a vanilla BlockClosure as in the first Cog
   release, BlockClosure laid out as 'outerContext, startpc, numArgs.
   FullBlockClosure is laid out as outerContext, method, numArgs, receiver. So
   either answer true if we're not supporting FullBlockClosure, or test the
   startpc/method field. */

/* StackInterpreter>>#isVanillaBlockClosure: */

static NoDbgRegParms int isVanillaBlockClosure(sqInt aClosure) {
  assert((isPointers(aClosure)) &&
         ((numSlotsOf(aClosure)) >= ClosureFirstCopiedValueIndex));
  return ((((longAt((void *)((aClosure + BaseHeaderSize) +
                             ((((usqInt)(ClosureStartPCIndex)
                                << (shiftForWord())))))))) &
           7) == 1);
}