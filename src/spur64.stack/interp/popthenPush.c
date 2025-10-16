/* Extracted from interp.c:58321 (function popthenPush). */

void
popthenPush(sqInt nItems, sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	longAtput((sp = GIV(stackPointer) + ((nItems - 1) * BytesPerWord)),oop);
	GIV(stackPointer) = sp;
}