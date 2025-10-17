/* Extracted from interp.c:55426 (function isWriteMediatedContextInstVarIndex).
 */

/*	Wrining any inst vars of a context must take account of potentially
        married contexts
        and set the state in the frame. Inst vars in subclasses don't need
        mediation; subclasses
        can't marry. */

/* StackInterpreter>>#isWriteMediatedContextInstVarIndex: */

int isWriteMediatedContextInstVarIndex(sqInt index) {
  return index <= ReceiverIndex;
}