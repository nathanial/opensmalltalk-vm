/* Extracted from interp.c:62654 (function reverseDisplayFromto). */

/*	Reverse the given range of Display pixels, rounded to whole word
   boundary. Used to give feedback during VM activities such as garbage
   collection when debugging. It is assumed that the given word range falls
   entirely within the first line of the Display. */

/* StackInterpreter>>#reverseDisplayFrom:to: */

static NoDbgRegParms void reverseDisplayFromto(sqInt startIndex,
                                               sqInt endIndex) {
  sqInt primFailCodeValue;
  sqInt ptr;
  sqInt reversed;
  sqInt toDoLimit;
  sqInt wordEndIndex;
  sqInt wordStartIndex;

  if (!(postGCUpdateDisplayBits())) {
    return;
  }
  if ((displayBits == 0) || (((((((sqInt)displayBits)) & (tagMask())) != 0)) ||
                             (displayDepth <= 0))) {
    return;
  }
  wordStartIndex = ((((startIndex < 0) ? 0 : startIndex)) * displayDepth) / 32;
  wordEndIndex =
      ((((endIndex < displayWidth) ? endIndex : displayWidth)) * displayDepth) /
      32;
  toDoLimit = (((sqInt)displayBits)) + (wordEndIndex * 4);
  for (ptr = ((((sqInt)displayBits)) + (wordStartIndex * 4)); ptr <= toDoLimit;
       ptr += 4) {
    reversed = (long32At((void *)(ptr))) ^ 0xFFFFFFFFU;
    long32Atput((void *)(ptr), reversed);
  }
  primFailCodeValue = primFailCode;

  /* begin initPrimCall */
  primFailCode = 0;
  updateDisplayLeftTopRightBottom(0, 0, displayWidth, 1);
  ioForceDisplayUpdate();
  primFailCode = primFailCodeValue;
}