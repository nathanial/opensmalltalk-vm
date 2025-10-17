/* Extracted from interp.c:63393 (function signed32BitIntegerFor). */

/*	Answer a full 32 bit integer object for the given integer value.
        N.B. Returning in each arm separately enables Slang inlining.
        /Don't/ return the ifTrue:ifFalse: unless Slang inlining of conditionals
        is fixed. */

/* StackInterpreter>>#signed32BitIntegerFor: */

sqInt signed32BitIntegerFor(sqInt integerValue) {
  return (((usqInt)(((int)integerValue)) << 3) | 1);
}