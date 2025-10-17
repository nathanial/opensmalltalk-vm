/* Extracted from interp.c:11036 (function vmIsInitialized). */

/*	Initialization is completed by initializing the stack zone on entry to the
	interpreter. Answer when this has been done. This allows e.g. the crash
	dump processing to
	be avoided until the system has been initialized, easing debugging of new
	ports as they fail in early system initialization attempts. */

	/* CogStackPages>>#vmIsInitialized */

int
vmIsInitialized(void)
{
	return !!pages;
}