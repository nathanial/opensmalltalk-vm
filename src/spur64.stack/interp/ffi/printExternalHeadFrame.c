/* Extracted from interp.c:59672 (function printExternalHeadFrame). */

/*	useful for VM debugging */

/* StackInterpreter>>#printExternalHeadFrame */

void printExternalHeadFrame(void) {
  printFrameWithSP(framePointer, stackPointer);
}