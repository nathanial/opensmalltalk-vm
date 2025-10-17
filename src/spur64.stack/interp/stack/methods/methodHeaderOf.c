/* Extracted from interp.c:40739 (function methodHeaderOf). */

/*	Answer the method header of a CompiledMethod object. */

/* SpurMemoryManager>>#methodHeaderOf: */

static sqInt methodHeaderOf(sqInt methodObj) {
  assert(isCompiledMethod(methodObj));
  return fetchPointerofObject(HeaderIndex, methodObj);
}