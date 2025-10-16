/* Extracted from interp.c:60268 (function printMethodDictionaryOf). */

/*	useful for VM debugging */

/* StackInterpreter>>#printMethodDictionaryOf: */

void printMethodDictionaryOf(sqInt behavior) {
  printMethodDictionary(fetchPointerofObject(MethodDictionaryIndex, behavior));
}