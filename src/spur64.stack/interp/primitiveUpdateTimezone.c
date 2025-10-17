/* Extracted from interp.c:25616 (function primitiveUpdateTimezone). */

/*	Update the VMs notion of the current timezone. The VM sets its notion
	of the timezone once at start-up. If one wants the VM to keep its notion
	up-to-date arrange to invoke this primitive periodically. */

	/* InterpreterPrimitives>>#primitiveUpdateTimezone */

static void
primitiveUpdateTimezone(void)
{
	ioUpdateVMTimezone();
}