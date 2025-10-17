/* Extracted from interp.c:63501 (function sizeOfLongStoreTempBytecode). */

/*	234		11101010	i i i i i i i i	Store Temporary Variable
 * #iiiiiiii */
/*	129 10000001 jjkkkkkk Store (Receiver Variable, Temporary Location,
        Illegal, Literal Variable) [jj] #kkkkkk */

/* StackInterpreter>>#sizeOfLongStoreTempBytecode: */

sqInt sizeOfLongStoreTempBytecode(sqInt methodHeader) { return 2; }