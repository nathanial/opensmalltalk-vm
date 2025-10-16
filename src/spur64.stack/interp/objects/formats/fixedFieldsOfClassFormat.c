/* Extracted from interp.c:35442 (function fixedFieldsOfClassFormat). */

/* SpurMemoryManager>>#fixedFieldsOfClassFormat: */

sqInt fixedFieldsOfClassFormat(sqInt classFormat) {
  return classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
}