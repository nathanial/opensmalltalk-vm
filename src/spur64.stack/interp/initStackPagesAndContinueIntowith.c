/* Extracted from interp.c:54839 (function initStackPagesAndContinueIntowith). */

/*	Initialize the stack pages and then invoke continuation. Use alloca'ed
	memory so that
	when we have a JIT its stack pointer will be on the native stack since
	alloca allocates
	memory on the stack. Certain thread systems use the native stack pointer
	as the
	frame ID so putting the stack anywhere else can confuse the thread system.
	
	This path is for the threaded VM where we may want to allocate the stack
	zone on
	the stack of a thread other than the main VM thread. */

	/* StackInterpreter>>#initStackPagesAndContinueInto:with: */

void
initStackPagesAndContinueIntowith(void (*continuation)(void *), void *argument)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt stackPageBytes;
    usqIntptr_t stackZoneBytes;
    void *theStackMemory;

	/* begin ensureInitializeStackZone */
	if (!GIV(pages)) {
		stackPageBytes = stackPageByteSize();
		stackZoneBytes = (GIV(numStackPages) * ((sizeof(CogStackPage)) + (stackPageByteSize()))) + BytesPerWord;
		theStackMemory = alloca(stackZoneBytes);
		memset(theStackMemory, 0, stackZoneBytes);
		initializeStacknumSlotspageSize(theStackMemory, stackZoneBytes / BytesPerWord, stackPageBytes / BytesPerWord);
	}
	continuation(argument);
}