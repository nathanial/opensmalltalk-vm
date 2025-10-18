/* Extracted helper matching StackInterpreter>>#pop:thenPushFloat: */

static inline void popthenPushFloat(sqInt nItems, sqInt floatOop) {
  char *sp;

  longAtput((sp = stackPointer + ((nItems - 1) * BytesPerWord)),
            floatOop);
  stackPointer = sp;
}
