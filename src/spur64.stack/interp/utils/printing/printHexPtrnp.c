/* Extracted from interp.c:59997 (function printHexPtrnp). */

/*	Print p in hex, unpadded, in the form 0x1234 (C)/16r1234 (here) */

/* StackInterpreter>>#printHexPtrnp: */

static sqInt printHexPtrnp(void *p) { return fprintf(transcript, "%p", p); }