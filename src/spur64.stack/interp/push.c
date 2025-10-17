/* Extracted from interp.c:61240 (function push). */

/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#push: */

void
push(sqInt object)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
	GIV(stackPointer) = sp;
}