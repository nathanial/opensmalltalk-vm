/* Extracted helper matching StackInterpreter>>#pop:thenPushBool: */

void popthenPushBool(sqInt nItems, sqInt booleanOop) {
  char *sp;

  longAtput((sp = stackPointer + ((nItems - 1) * BytesPerWord)), booleanOop);
  stackPointer = sp;
}
