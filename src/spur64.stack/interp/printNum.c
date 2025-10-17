/* Extracted from interp.c:60343 (function printNum). */

/* StackInterpreter>>#printNum: */

static NoDbgRegParms void printNum(sqInt n) {
  fprintf(transcript, "%" PRIdSQINT "", ((sqInt)n));
}