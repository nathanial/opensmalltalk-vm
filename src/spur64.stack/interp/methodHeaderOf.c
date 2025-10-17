/* Extracted from interp.c:40739 (function methodHeaderOf). */

/*	Answer the method header of a CompiledMethod object. */

/* SpurMemoryManager>>#methodHeaderOf: */

static NoDbgRegParms sqInt methodHeaderOf(sqInt methodObj) {
  assert(isCompiledMethod(methodObj));
  return longAt((void *)((methodObj + BaseHeaderSize) +
                         ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
}