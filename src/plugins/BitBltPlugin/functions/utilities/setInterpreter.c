/* Extracted from BitBltPlugin.c:5776 (function setInterpreter). */
/* InterpreterPlugin>>#setInterpreter: */
/*	WARNING: For WarpBlt w/ smoothing the source depth is wrong here! */

EXPORT(sqInt)
setInterpreter(struct VirtualMachine *anInterpreter) {
  sqInt ok;

  interpreterProxy = anInterpreter;

  /* This may seem tautological, but in a real plugin it checks that the VM
     provides the version the plugin was compiled against which is the version
     the plugin expects. */
  ok = ((interpreterProxy->majorVersion()) == (VM_PROXY_MAJOR)) &&
       ((interpreterProxy->minorVersion()) >= (VM_PROXY_MINOR));
  if (ok) {

  }
  return ok;
}
