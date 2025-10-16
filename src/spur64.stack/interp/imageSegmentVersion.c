/* Extracted from interp.c:36237 (function imageSegmentVersion). */

static sqInt
imageSegmentVersion(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt wholeWord;

	/* first data word, 'does' */
	wholeWord = long32At((void *)((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SelectorDoesNotUnderstand) << (shiftForWord()))))))) + BaseHeaderSize));
	return 68021 /* imageFormatVersion */ | (wholeWord & 0xFF000000U);
}