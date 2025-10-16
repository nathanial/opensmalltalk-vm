/* Extracted from interp.c:11892 (function primitiveArrayBecome). */

/*	Invoke the two-way become primitive.
        We must at least flush the method cache here, to eliminate stale
        references to mutated classes and/or selectors.

        If in the CoInterpreter, we must deal with jitted methods being
   becommed. In the conception of the abstract VM, unless a CompiledMethod is
   becommed to one with equivalent bytecode contexts referring to the becommed
        method will likely fetch invalid bytecodes on resumption. The
        responsibility for validity here lies with the user of
        the become primitive, not the VM. So one could imagine checking for
        methods becomming equivalent methods
        and updating Cog methods to refer to their becommed duals. But that
        requires machinery to compare two
        compiled methods to check if their code is equivalent. A much simpler
        approach, which also gets the VM to fail
        in a less confusing place if it is going to fail because the programmer
        has not ensured compiled code validity
        across become, is to simply divorce all frames and map context pcs back
   to bytecode pcs when becomming jitted methods. However, mapping native pcs in
   contexts whose methods are flagged for become will not ensure that pcs are
   mapped reliably. Consider this arc: a context on a method that has been
   jitted exists with a native pc. The method is unjitted to make room for other
   methods to be jitted. The method is becommed. So the scan is only effective
   for methods in the jit. Slowing down become so that a rare case may fail more
   comprehensibly, when its going to fail anyway, is a waste of effort. So we do
   divorce frames (since machine code frames refer to CogMethods, not methods),
   but don't scan the entire heap looking for native pcs in contexts. Hence the
   primitive must be prepared to have its calling frame divorced. We store the
   context for the top frame in activeProcess. BTW, as of late '22/early '23
   this is only done for Spur. See preBecomeAction: and postBecomeAction: */

/* InterpreterPrimitives>>#primitiveArrayBecome */

static void primitiveArrayBecome(void) {
  sqInt arg;
  sqInt ec;
  sqInt rcvr;

  arg = longAt(stackPointer);
  rcvr = longAt(stackPointer + (1 * BytesPerWord));
  ec = becomewithtwoWaycopyHash(rcvr, arg, 1, 0);

  /* begin primitiveBecomeReturn: */
  if (ec) {
    /* primitiveFailFor: */
    primFailCode = ec;
  } else {
    /* begin methodReturnReceiver */
    assert(!((failed())));
    stackPointer += argumentCount * BytesPerWord;
  }
}