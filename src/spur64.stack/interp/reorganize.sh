#!/bin/bash
# Script to reorganize interp directory files into subdirectories

cd "$(dirname "$0")"

echo "Starting reorganization..."

# PRIMITIVES - Arithmetic
echo "Moving arithmetic primitives..."
mv primitive{Add,Subtract,Multiply,Divide,Div,Mod,Quo,Rem}*.c primitives/arithmetic/ 2>/dev/null
mv primitive{GreaterThan,LessThan,GreaterOrEqual,LessOrEqual,Equal,NotEqual}*.c primitives/arithmetic/ 2>/dev/null
mv primitiveIdentical*.c primitiveNotIdentical*.c primitives/arithmetic/ 2>/dev/null
mv primitive{Float,SmallFloat}{Add,Subtract,Multiply,Divide,Equal,NotEqual,LessThan,GreaterThan,LessOrEqual,GreaterOrEqual}*.c primitives/arithmetic/ 2>/dev/null
mv primitive{Float,SmallFloat}{Sine,Cosine,Arctan,Exp,LogN,SquareRoot,Exponent,FractionalPart,Truncated}*.c primitives/arithmetic/ 2>/dev/null
mv primitiveAsFloat*.c primitiveTimesTwoPower*.c primitiveHashMultiply*.c primitives/arithmetic/ 2>/dev/null

# PRIMITIVES - Bitwise
echo "Moving bitwise primitives..."
mv primitiveBit*.c primitiveHighBit*.c primitives/bitwise/ 2>/dev/null

# PRIMITIVES - Collections
echo "Moving collection primitives..."
mv primitiveAt.c primitiveAtPut.c primitiveSize.c primitiveString*.c primitives/collections/ 2>/dev/null
mv primitiveCompareBytes*.c primitiveIndexOf*.c primitiveScanCharacters*.c primitives/collections/ 2>/dev/null
mv primitiveFloatArray*.c primitives/collections/ 2>/dev/null

# PRIMITIVES - Objects
echo "Moving object primitives..."
mv primitiveNew*.c primitiveClone*.c primitiveCopyObject*.c primitiveConstantFill*.c primitives/objects/ 2>/dev/null
mv primitive{Pinned,Pin,IsPin}*.c primitives/objects/ 2>/dev/null
mv primitiveArray Become*.c primitives/objects/ 2>/dev/null
mv primitive{All,Some,Next}{Instance,Object}*.c primitives/objects/ 2>/dev/null
mv primitiveIdentityHash*.c primitiveBehaviorHash*.c primitiveMaxIdentityHash*.c primitiveSetOrHasIdentityHash*.c primitives/objects/ 2>/dev/null
mv primitiveClass*.c primitiveChangeClass*.c primitiveAdoptInstance*.c primitives/objects/ 2>/dev/null
mv primitiveSizeInBytes*.c primitiveObjectPointsTo*.c primitiveObjectsAccessibleFromRoots*.c primitivePathToUsing*.c primitives/objects/ 2>/dev/null
mv primitive{Get,Set}Immutability*.c primitiveMakePoint*.c primitives/objects/ 2>/dev/null

# PRIMITIVES - Control (Closures, method invocation)
echo "Moving control primitives..."
mv primitiveClosure*.c primitiveFullClosure*.c primitives/control/ 2>/dev/null
mv primitivePerform*.c primitiveExecuteMethod*.c primitiveInvokeObjectAsMethod*.c primitives/control/ 2>/dev/null
mv primitiveDoNamedPrimitiveWithArgs*.c primitiveDoPrimitiveWithArgs*.c primitives/control/ 2>/dev/null
mv primitiveContext*.c primitiveStoreStackp*.c primitiveFindHandlerContext*.c primitiveFindNextUnwindContext*.c primitives/control/ 2>/dev/null

# PRIMITIVES - Processes
echo "Moving process primitives..."
mv primitive{Suspend,Resume,Yield,Signal,Wait}*.c primitiveRelinquishProcessor*.c primitiveTerminateTo*.c primitives/processes/ 2>/dev/null
mv primitive{Enter,Exit,TestAndSetOwnershipOf}CriticalSection*.c primitives/processes/ 2>/dev/null
mv primitiveInputSemaphore*.c primitiveInterruptSemaphore*.c primitiveLowSpaceSemaphore*.c primitives/processes/ 2>/dev/null

# PRIMITIVES - System
echo "Moving system primitives..."
mv primitiveSnapshot*.c primitiveQuit*.c primitiveExitToDebugger*.c primitiveCrashVM*.c primitiveVoidVMState*.c primitiveVoidReceiver*.c primitives/system/ 2>/dev/null
mv primitive{Full,Incremental}GC*.c primitiveBytesLeft*.c primitiveGrowMemoryByAtLeast*.c primitiveSignalAtBytesLeft*.c primitives/system/ 2>/dev/null
mv primitiveFetchNextMourner*.c primitive{Load,Store}ImageSegment*.c primitives/system/ 2>/dev/null
mv primitiveVMParameter*.c primitiveImageFormatVersion*.c primitiveIsBigEnder*.c primitives/system/ 2>/dev/null
mv primitiveBytecodeSetsAvailable*.c primitiveMultipleBytecodeSetsActive*.c primitives/system/ 2>/dev/null
mv primitiveInterruptChecksPerMSec*.c primitiveDisablePowerManager*.c primitiveGetSetFFIExceptionHandling*.c primitives/system/ 2>/dev/null
mv primitiveImageName*.c primitiveVMPath*.c primitive{Get,Set}Attribute*.c primitiveGetenv*.c primitiveGet*.c primitiveSet*.c primitives/system/ 2>/dev/null
mv primitive{List,Unload}*Module*.c primitiveFlushExternalPrimitives*.c primitives/system/ 2>/dev/null
mv primitiveFlushCache*.c primitiveIndexOfMethodheader*.c primitives/system/ 2>/dev/null
mv primitiveMethod*.c primitiveNoop*.c primitiveCompareWith*.c primitives/system/ 2>/dev/null
mv primitiveErrorTable*.c primitiveFailureCode*.c primitives/system/ 2>/dev/null
mv primitiveImmediateAsInteger*.c primitiveAsCharacter*.c primitives/system/ 2>/dev/null
mv primitiveSpecialObjectsOop*.c primitives/system/ 2>/dev/null

# PRIMITIVES - Time
echo "Moving time primitives..."
mv primitive{Millisecond,Second,HighRes,LocalMicrosecond,UTCMicrosecond,CoarseLocalMicrosecond,CoarseUTCMicrosecond}Clock*.c primitives/time/ 2>/dev/null
mv primitiveUtc*.c primitiveUpdateTimezone*.c primitives/time/ 2>/dev/null
mv primitiveSignalAt{Milliseconds,UTCMicroseconds}*.c primitiveHeartbeatFrequency*.c primitives/time/ 2>/dev/null

# PRIMITIVES - I/O
echo "Moving I/O primitives..."
mv primitiveBeDisplay*.c primitiveScreen*.c primitiveForceDisplayUpdate*.c primitiveDeferDisplayUpdates*.c primitives/io/ 2>/dev/null
mv primitiveSetFullScreen*.c primitiveSetDisplayMode*.c primitiveTestDisplayDepth*.c primitiveShowDisplayRect*.c primitives/io/ 2>/dev/null
mv primitiveBeCursor*.c primitiveFormPrint*.c primitives/io/ 2>/dev/null
mv primitiveGetNextEvent*.c primitiveInputWord*.c primitiveEventProcessingControl*.c primitiveSetInterruptKey*.c primitives/io/ 2>/dev/null
mv primitiveBeep*.c primitiveClipboardText*.c primitives/io/ 2>/dev/null

# PRIMITIVES - FFI
echo "Moving FFI primitives..."
mv primitiveCalloutToFFI*.c primitiveExternalCall*.c primitiveFailForFFIExceptionat*.c primitives/ffi/ 2>/dev/null

# PRIMITIVES - Profiling
echo "Moving profiling primitives..."
mv primitiveProfile*.c primitiveControlVMProfiling*.c primitiveClearVMProfile*.c primitiveVMProfileSamplesInto*.c primitiveClockLogAddresses*.c primitives/profiling/ 2>/dev/null

# PRIMITIVES - Remaining (catch any we missed)
mv primitive*.c primitives/ 2>/dev/null

echo "Primitives done."

# GC - Marking
echo "Moving GC marking files..."
mv mark*.c gc/marking/ 2>/dev/null
mv unmark*.c gc/marking/ 2>/dev/null

# GC - Scavenging
echo "Moving GC scavenging files..."
mv scaveng*.c gc/scavenging/ 2>/dev/null
mv doScavenge*.c gc/scavenging/ 2>/dev/null

# GC - Compaction
echo "Moving GC compaction files..."
mv compact*.c gc/compaction/ 2>/dev/null
mv *Compaction*.c gc/compaction/ 2>/dev/null
mv *Compacting*.c gc/compaction/ 2>/dev/null
mv slidingCompaction*.c gc/compaction/ 2>/dev/null

# GC - Allocation
echo "Moving GC allocation files..."
mv allocate*.c gc/allocation/ 2>/dev/null
mv *Allocat*.c gc/allocation/ 2>/dev/null
mv instantiate*.c gc/allocation/ 2>/dev/null
mv eeInstantiate*.c gc/allocation/ 2>/dev/null

# GC - Free lists
echo "Moving GC free list files..."
mv *Free*.c gc/freelists/ 2>/dev/null
mv *freeList*.c gc/freelists/ 2>/dev/null
mv *FreeChunk*.c gc/freelists/ 2>/dev/null

# GC - Weak references
echo "Moving GC weak/ephemeron files..."
mv *Weak*.c gc/weak/ 2>/dev/null
mv *weakling*.c gc/weak/ 2>/dev/null
mv *Ephemeron*.c gc/weak/ 2>/dev/null
mv *ephemeron*.c gc/weak/ 2>/dev/null
mv fireEphemeron*.c gc/weak/ 2>/dev/null
mv processWeaklings*.c gc/weak/ 2>/dev/null
mv *Mourn*.c gc/weak/ 2>/dev/null
mv queueMourner*.c gc/weak/ 2>/dev/null

# GC - Segments
echo "Moving GC segment files..."
mv *Segment*.c gc/segments/ 2>/dev/null
mv *segment*.c gc/segments/ 2>/dev/null

# GC - Spaces
echo "Moving GC space files..."
mv *Space*.c gc/spaces/ 2>/dev/null
mv *space*.c gc/spaces/ 2>/dev/null
mv *Eden*.c gc/spaces/ 2>/dev/null
mv *Survivor*.c gc/spaces/ 2>/dev/null

# GC - Remembered set
echo "Moving GC remembered set files..."
mv remember*.c gc/remembered/ 2>/dev/null
mv *Remember*.c gc/remembered/ 2>/dev/null
mv *remembered*.c gc/remembered/ 2>/dev/null

# GC - Pinning
echo "Moving GC pinning files..."
mv pin*.c gc/pinning/ 2>/dev/null
mv *Pin*.c gc/pinning/ 2>/dev/null
mv *Pinned*.c gc/pinning/ 2>/dev/null
mv *Pinning*.c gc/pinning/ 2>/dev/null

# GC - Remaining
mv *GC*.c gc/ 2>/dev/null
mv fullGC*.c gc/ 2>/dev/null
mv *Forward*.c gc/ 2>/dev/null
mv forward*.c gc/ 2>/dev/null
mv follow*.c gc/ 2>/dev/null
mv bridge*.c gc/ 2>/dev/null
mv *Bridge*.c gc/ 2>/dev/null
mv *Leak*.c gc/ 2>/dev/null
mv copyAnd*.c gc/ 2>/dev/null

echo "GC files done."

# STACK - Manipulation
echo "Moving stack manipulation files..."
mv push*.c stack/manipulation/ 2>/dev/null
mv pop*.c stack/manipulation/ 2>/dev/null
mv stackValue*.c stack/manipulation/ 2>/dev/null
mv stackTop*.c stack/manipulation/ 2>/dev/null
mv stackInteger*.c stack/manipulation/ 2>/dev/null
mv stackFloat*.c stack/manipulation/ 2>/dev/null
mv stackObject*.c stack/manipulation/ 2>/dev/null

# STACK - Frames
echo "Moving stack frame files..."
mv frame*.c stack/frames/ 2>/dev/null

# STACK - Pages
echo "Moving stack page files..."
mv *StackPage*.c stack/pages/ 2>/dev/null
mv newStackPage*.c stack/pages/ 2>/dev/null
mv freeStackPage*.c stack/pages/ 2>/dev/null

# STACK - Contexts
echo "Moving context files..."
mv *Context*.c stack/contexts/ 2>/dev/null
mv *context*.c stack/contexts/ 2>/dev/null
mv loadInitialContext*.c stack/contexts/ 2>/dev/null
mv cloneContext*.c stack/contexts/ 2>/dev/null
mv ensureCallerContext*.c stack/contexts/ 2>/dev/null
mv divorce*.c stack/contexts/ 2>/dev/null
mv marry*.c stack/contexts/ 2>/dev/null

# STACK - Methods
echo "Moving method files..."
mv method*.c stack/methods/ 2>/dev/null
mv *Method*.c stack/methods/ 2>/dev/null
mv executeNewMethod*.c stack/methods/ 2>/dev/null

# STACK - Lookup
echo "Moving lookup files..."
mv lookup*.c stack/lookup/ 2>/dev/null
mv *Lookup*.c stack/lookup/ 2>/dev/null
mv findSelector*.c stack/lookup/ 2>/dev/null
mv *Cache*.c stack/lookup/ 2>/dev/null

# STACK - Closures
echo "Moving closure files..."
mv *Closure*.c stack/closures/ 2>/dev/null
mv *closure*.c stack/closures/ 2>/dev/null
mv argumentCountOfClosure*.c stack/closures/ 2>/dev/null

# STACK - Callbacks
echo "Moving callback files..."
mv callback*.c stack/callbacks/ 2>/dev/null

# STACK - Validation
echo "Moving stack validation files..."
mv checkStack*.c stack/validation/ 2>/dev/null
mv validStack*.c stack/validation/ 2>/dev/null
mv assertValidExecutionPointers*.c stack/validation/ 2>/dev/null

# STACK - Remaining
mv *Activation*.c stack/ 2>/dev/null
mv activeProcess*.c stack/ 2>/dev/null

echo "Stack files done."

# OBJECTS - Access
echo "Moving object access files..."
mv fetch*.c objects/access/ 2>/dev/null
mv store*.c objects/access/ 2>/dev/null
mv stObjectat*.c objects/access/ 2>/dev/null
mv firstIndexableField*.c objects/access/ 2>/dev/null
mv firstFixedField*.c objects/access/ 2>/dev/null

# OBJECTS - Predicates
echo "Moving object predicates..."
mv is*.c objects/predicates/ 2>/dev/null
mv has*.c objects/predicates/ 2>/dev/null
mv should*.c objects/predicates/ 2>/dev/null

# OBJECTS - Formats
echo "Moving object format files..."
mv format*.c objects/formats/ 2>/dev/null
mv *Format*.c objects/formats/ 2>/dev/null
mv instSpecOf*.c objects/formats/ 2>/dev/null
mv byteFormatFor*.c objects/formats/ 2>/dev/null

# OBJECTS - Classes
echo "Moving class files..."
mv class*.c objects/classes/ 2>/dev/null
mv *Class*.c objects/classes/ 2>/dev/null
mv superclass*.c objects/classes/ 2>/dev/null
mv *ClassTag*.c objects/classes/ 2>/dev/null
mv *ClassTable*.c objects/classes/ 2>/dev/null

# OBJECTS - Headers
echo "Moving header files..."
mv header*.c objects/headers/ 2>/dev/null
mv *Header*.c objects/headers/ 2>/dev/null

# OBJECTS - Hashing
echo "Moving hash files..."
mv *Hash*.c objects/hashing/ 2>/dev/null
mv *hash*.c objects/hashing/ 2>/dev/null
mv identityHashOf*.c objects/hashing/ 2>/dev/null

# OBJECTS - Sizes
echo "Moving size files..."
mv *Size*.c objects/sizes/ 2>/dev/null
mv *size*.c objects/sizes/ 2>/dev/null
mv slotSizeOf*.c objects/sizes/ 2>/dev/null
mv byteSizeOf*.c objects/sizes/ 2>/dev/null
mv numSlots*.c objects/sizes/ 2>/dev/null
mv numBytes*.c objects/sizes/ 2>/dev/null
mv lengthOf*.c objects/sizes/ 2>/dev/null

# OBJECTS - Conversion
echo "Moving conversion files..."
mv *ValueOf*.c objects/conversion/ 2>/dev/null
mv *valueOf*.c objects/conversion/ 2>/dev/null
mv integerObjectOf*.c objects/conversion/ 2>/dev/null
mv floatObjectOf*.c objects/conversion/ 2>/dev/null
mv characterObjectOf*.c objects/conversion/ 2>/dev/null
mv booleanObjectOf*.c objects/conversion/ 2>/dev/null
mv *For.c objects/conversion/ 2>/dev/null

# OBJECTS - Traversal
echo "Moving traversal files..."
mv objectAfter*.c objects/traversal/ 2>/dev/null
mv objectBefore*.c objects/traversal/ 2>/dev/null
mv *ObjectAfter*.c objects/traversal/ 2>/dev/null
mv addressAfter*.c objects/traversal/ 2>/dev/null
mv accessibleObject*.c objects/traversal/ 2>/dev/null

# OBJECTS - Copying
echo "Moving copying files..."
mv clone*.c objects/copying/ 2>/dev/null
mv copy*.c objects/copying/ 2>/dev/null
mv remap*.c objects/copying/ 2>/dev/null
mv become*.c objects/copying/ 2>/dev/null

echo "Object files done."

# EXECUTION
echo "Moving execution files..."
# Keep interpret.c in execution root
mv interpret.c execution/ 2>/dev/null
mv *Bytecode*.c execution/bytecodes/ 2>/dev/null
mv *bytecode*.c execution/bytecodes/ 2>/dev/null
mv executeNew*.c execution/dispatch/ 2>/dev/null
mv *Interrupt*.c execution/interrupts/ 2>/dev/null
mv *interrupt*.c execution/interrupts/ 2>/dev/null
mv checkForEvents*.c execution/interrupts/ 2>/dev/null
mv forceInterruptCheck*.c execution/interrupts/ 2>/dev/null

echo "Execution files done."

# FFI
echo "Moving FFI files..."
mv *ExternalCall*.c ffi/calls/ 2>/dev/null
mv *externalCall*.c ffi/calls/ 2>/dev/null
mv callbackEnter*.c ffi/callbacks/ 2>/dev/null
mv callbackLeave*.c ffi/callbacks/ 2>/dev/null
mv *Callback*.c ffi/callbacks/ 2>/dev/null
mv link*.c ffi/linking/ 2>/dev/null
mv *External*.c ffi/ 2>/dev/null

echo "FFI files done."

# PROCESSES
echo "Moving process scheduling files..."
mv *Resume*.c processes/scheduling/ 2>/dev/null
mv *resume*.c processes/scheduling/ 2>/dev/null
mv *Process*.c processes/scheduling/ 2>/dev/null
mv *process*.c processes/scheduling/ 2>/dev/null
mv synchronousSignal*.c processes/semaphores/ 2>/dev/null
mv *Semaphore*.c processes/semaphores/ 2>/dev/null
mv *semaphore*.c processes/semaphores/ 2>/dev/null
mv *CriticalSection*.c processes/synchronization/ 2>/dev/null

echo "Process files done."

# IMAGE
echo "Moving image files..."
mv snapshot*.c image/snapshot/ 2>/dev/null
mv *Snapshot*.c image/snapshot/ 2>/dev/null
mv writeImage*.c image/snapshot/ 2>/dev/null
mv readImage*.c image/loading/ 2>/dev/null
mv *ImageSegment*.c image/segments/ 2>/dev/null
mv ensureImageFormat*.c image/ 2>/dev/null

echo "Image files done."

# PROFILING
echo "Moving profiling files..."
mv *Profile*.c profiling/sampling/ 2>/dev/null
mv *profile*.c profiling/sampling/ 2>/dev/null
mv *TraceLog*.c profiling/tracing/ 2>/dev/null
mv *traceLog*.c profiling/tracing/ 2>/dev/null
mv dumpPrimTraceLog*.c profiling/tracing/ 2>/dev/null
mv *Stats*.c profiling/stats/ 2>/dev/null
mv *Count*.c profiling/stats/ 2>/dev/null

echo "Profiling files done."

# UTILS - Printing
echo "Moving utility printing files..."
mv print*.c utils/printing/ 2>/dev/null
mv *Print*.c utils/printing/ 2>/dev/null
mv dump*.c utils/printing/ 2>/dev/null
mv show*.c utils/printing/ 2>/dev/null
mv longPrint*.c utils/printing/ 2>/dev/null
mv shortPrint*.c utils/printing/ 2>/dev/null

# UTILS - Validation
echo "Moving utility validation files..."
mv check*.c utils/validation/ 2>/dev/null
mv assert*.c utils/validation/ 2>/dev/null
mv valid*.c utils/validation/ 2>/dev/null
mv whereIs*.c utils/validation/ 2>/dev/null

# UTILS - Config
echo "Moving utility config files..."
mv default*.c utils/config/ 2>/dev/null
mv max*.c utils/config/ 2>/dev/null
mv min*.c utils/config/ 2>/dev/null
mv get*.c utils/config/ 2>/dev/null
mv set*.c utils/config/ 2>/dev/null

# UTILS - Errors
echo "Moving utility error files..."
mv *Fail*.c utils/errors/ 2>/dev/null
mv *fail*.c utils/errors/ 2>/dev/null
mv warning*.c utils/errors/ 2>/dev/null
mv error*.c utils/errors/ 2>/dev/null
mv eek*.c utils/errors/ 2>/dev/null

echo "Utility files done."

# Move any remaining .c files to utils
echo "Moving remaining files to utils..."
mv *.c utils/ 2>/dev/null

echo "Reorganization complete!"
echo "Files have been moved to subdirectories."
echo "Next step: Update interp.c to use new include paths."
