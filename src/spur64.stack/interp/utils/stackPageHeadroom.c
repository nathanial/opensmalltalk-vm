/* Extracted from interp.c:64360 (function stackPageHeadroom). */

/*	Return a minimum amount of headroom for each stack page (in bytes).
        In a JIT the stack has to have room for interrupt handlers which will
   run on the stack. In the interpreter we don't actually need any headroom. */

/* StackInterpreter>>#stackPageHeadroom */

static sqInt stackPageHeadroom(void) { return 0; }