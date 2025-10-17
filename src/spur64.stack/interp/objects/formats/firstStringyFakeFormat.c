/* Extracted from interp.c:35428 (function firstStringyFakeFormat). */

/*	A fake format for the interpreter used to mark indexable strings in
        the interpreter's at cache. This is larger than any format. */

/* SpurMemoryManager>>#firstStringyFakeFormat */

static sqInt firstStringyFakeFormat(void) { return 32; }