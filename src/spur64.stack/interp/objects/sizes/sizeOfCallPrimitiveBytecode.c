/* Extracted from interp.c:63489 (function sizeOfCallPrimitiveBytecode). */

/*	Answer the size of the CallPrimitive bytecode that may be used to store
   a method's primitive.
 */
/*	NewsqueakV4:	249	11111001	iiiiiiii	jjjjjjjj
   Call Primitive #iiiiiiii + (jjjjjjjj * 256)
 */
/*	SistaV1:		248	11111000 iiiiiiii	mssjjjjj
   Call Primitive #iiiiiiii + ( jjjjj * 256) */
/*	V3+Closures:	139	11101111	iiiiiiii	jjjjjjjj
   Call Primitive #iiiiiiii + (jjjjjjjj * 256)
 */

/* StackInterpreter>>#sizeOfCallPrimitiveBytecode: */

sqInt sizeOfCallPrimitiveBytecode(sqInt methodHeader) { return 3; }