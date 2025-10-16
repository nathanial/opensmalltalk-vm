/* Extracted from interp.c:57705 (function methodClassAssociationOf). */

/* StackInterpreter>>#methodClassAssociationOf: */

sqInt methodClassAssociationOf(sqInt methodPointer) {
  sqInt offset;

  offset = (literalCountOf(methodPointer)) - 1;

  /* begin literal:ofMethod: */
  return fetchPointerofObject(offset + LiteralStart, methodPointer);
}