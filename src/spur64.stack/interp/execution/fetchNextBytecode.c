/* Extracted from interp.c (function fetchNextBytecode). */

/* StackInterpreter>>#fetchNextBytecode */

static inline sqInt fetchNextBytecode(char **localIPP,
                                      sqInt bytecodeSetSelector) {
  return (byteAt(++(*localIPP))) + bytecodeSetSelector;
}
