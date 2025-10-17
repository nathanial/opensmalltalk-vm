/* Extracted helper matching ObjectMemory>>#booleanObjectOf: */

sqInt booleanObjectOf(sqInt flag) {
  return flag ? trueObj : falseObj;
}
