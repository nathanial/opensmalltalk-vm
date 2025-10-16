/* Extracted from interp.c:21251 (function primitiveQuit). */

/* InterpreterPrimitives>>#primitiveQuit */

static void primitiveQuit(void) {
  ioExitWithErrorCode((argumentCount == 1 ? ((longAt(stackPointer)) >> 3) : 0));
}