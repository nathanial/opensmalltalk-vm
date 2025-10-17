/* Extracted from interp.c:54138 (function getCogVMFeatureFlags). */

/*	Answer an array of flags indicating various optional features of the Cog
	VM. If the bit is set then...
	Bit 0: supports two bytecode sets (MULTIPLEBYTECODESETS)
	Bit 1: supports immutablity (IMMUTABILITY)
	Bit 2: suffers from a UNIX setitimer signal-based heartbeat
	Bit 3: the VM provides cross-platform bit-identical floating point
	Bit 4: the VM can catch exceptions in FFI calls and answer them as
	primitive failures
	Bit 5: the VM has suspend primitives 568 & 578 which back up a process to
	before the wait if it was waiting on a condition variable */

	/* StackInterpreter>>#getCogVMFeatureFlags */

static sqInt
getCogVMFeatureFlags(void)
{
	return (((usqInt)(((((1 + (
#if IMMUTABILITY
	(IMMUTABILITY
			? 2
			: 0)
#else
	0
#endif
	)) + (
#if ITIMER_HEARTBEAT
	4
#else
	0
#endif
	)) + (
#if BIT_IDENTICAL_FLOATING_POINT
	8
#else
	0
#endif
	)) + ((ioCanCatchFFIExceptions()
		? 16
		: 0))) + 32) << 3) | 1);
}