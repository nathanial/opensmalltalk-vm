/* Extracted helper matching StackInterpreter>>#externalizeIPandSP */

void externalizeIPandSP(char *ip, char *sp, char *fp) {
  instructionPointer = (usqInt)ip;
  stackPointer = sp;
  framePointer = fp;
}
