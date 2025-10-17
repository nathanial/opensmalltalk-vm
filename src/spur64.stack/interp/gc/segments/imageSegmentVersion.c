/* Extracted from interp.c:36237 (function imageSegmentVersion). */

/*	a more complex version that tells both the word reversal and the
        endianness of the machine
        it came from. Low half of word is e.g. 6521. Top byte is top byte of
        #doesNotUnderstand: on
        this machine. ($d on the Mac or $s on the PC) */

/* SpurMemoryManager>>#imageSegmentVersion */

static sqInt imageSegmentVersion(void) {
  sqInt wholeWord;

  /* first data word, 'does' */
  wholeWord =
      long32At((void *)((longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                                         ((((usqInt)(SelectorDoesNotUnderstand)
                                            << (shiftForWord()))))))) +
                        BaseHeaderSize));
  return 68021 /* imageFormatVersion */ | (wholeWord & 0xFF000000U);
}