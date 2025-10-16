/* Extracted from interp.c:54138 (function getCogVMFeatureFlags). */

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