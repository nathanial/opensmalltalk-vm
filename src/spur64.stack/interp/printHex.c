/* Extracted from interp.c:60013 (function printHex). */

/*	Print n in hex, in the form ' 0x1234', padded to a width of 10 characters
	in 32-bits ('0x' + 8 nibbles) or 18 characters in 64-bits ('0x' + 16
	nibbles). In the simulator use 16r as the prefix, padding to 11 characters
	in 32-bits
	or 19 characters in 64-bits. */

	/* StackInterpreter>>#printHex: */

void
printHex(usqInt n)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	fprintf(GIV(transcript),
			"%16p" /* pst: */,
			((void *)n));
}