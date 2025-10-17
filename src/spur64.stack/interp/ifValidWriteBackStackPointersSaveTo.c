/* Extracted from interp.c:54787 (function ifValidWriteBackStackPointersSaveTo). */

/*	This is for low-level error reporting. If either of the C stack pointers
	are pointing into the stack zone then write them back to framePointer
	and/or stackPointer so that the stack backtrace will be up to date. Write
	their original values through savedFPP & savedSPP if non-null.
	This is a noop in the stack VM since the C stack pointers are always
	elsewhere (e.g., in some C function running the interpreter). */

	/* StackInterpreter>>#ifValidWriteBackStack:Pointers:Save:To: */

void
ifValidWriteBackStackPointersSaveTo(void *theCFP, void *theCSP, char **savedFPP, char **savedSPP)
{
}