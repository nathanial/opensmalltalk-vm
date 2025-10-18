/* Extracted from interp.c (function fetchNextBytecode). */

/* StackInterpreter>>#fetchNextBytecode */

static sqInt fetchNextBytecode(char **localIPP, sqInt bytecodeSetSelector) {
  return (byteAt(++(*localIPP))) + bytecodeSetSelector;
}
