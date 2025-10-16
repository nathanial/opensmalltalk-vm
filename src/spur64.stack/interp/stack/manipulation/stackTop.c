/* Extracted from interp.c:64459 (function stackTop). */

/* StackInterpreter>>#stackTop */

sqInt stackTop(void) { return longAt(stackPointer); }