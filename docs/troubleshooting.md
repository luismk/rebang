This is a guide to troubleshooting mismatching decompilations.

The general best advice for starting here is:

- Always consult the PDB. Use `cvdump` and IDA/Ghidra. The PDB contains many useful hints.
- When working on a new unit, before even cutting over to the .cpp build, start with the prerequisites. Confirm names using PDB addresses, contributions, references wherever possible.
- Many symbols have the wrong COMDAT flags in the delink and will need to be marked `any`.
- Many symbols are not properly named in the delink and need to be matched up when you discover section(s) that should be elided by COMDAT.
- If you run into a wall, start digging deeper into the compiler behavior itself. We're not going to finish this by getting lucky with thousands of random guesses :)

# `fatal error LNK1169: one or more multiply defined symbols found`

You may run into some issues caused by COMDAT (common data) flags. The final linked executable loses the necessary information to correctly reconstruct section flags. This will result in duplicate symbol errors. To fix this, sections incorrectly marked `IMAGE_COMDAT_SELECT_NODUPLICATES` may need to be re-written to `IMAGE_COMDAT_SELECT_ANY`.

For example:

```shell
$ python tools/coffsym.py set-selection ./source/client/Wangreal/source/wview.obj '??1WView@@UAE@XZ' any
```

# VC7.1 compiler internals

The VC7.1 compiler is primarily broken into three parts:

- `c1.dll`: The C frontend. C goes in, CIL goes out.
- `c1xx.dll`: The C++ frontend. C++ goes in, CIL goes out.
- `c2.dll`: The compiler backend, also known as the Universal Tuple Compiler.

For our purposes, `c1` and `c1xx` are not terribly interesting. We're more interested in `c2`/the UTC, as it is the part of MSVC where code generation happens. If you want to be able to make decompilations efficiently, you need to understand and be able to trace `c2` behavior.

## Register allocation overview

Callee-saved registers are assigned first by rank then colored in rounds. For each block, a live candidate that is referenced gains roughly `2 * refs * pressure * weight`, whereas a live-but-unreferenced candidate loses `pressure * weight`, where block weight is `2 ^ loop depth`. The first round colors candidates with a positive rank, in rank order, and anything with a free register gets one. The rest go through split rounds, one candidate at a time. Coloring a candidate that empties another candidate's register mask starts a split round immediately. Worth is `(2 * weight * use) - (2 * weight * reload)`. A candidate whose worth is `< 0` is dropped or split. Allocation differences can occur because of seemingly inoccuous declarations or assignments; declaring variables earlier and assigning them later can impact register allocation even when the emitted code is identical.

## Emission order overview

From the frontend perspective, functions are emitted mostly in source order, except template instantiations, which are _usually_ emitted at the end (although, as an exception, virtual functions from templated classes seem to be instantiated inline with the class itself.) This impacts the IL offset, which the backend will use for its ordering. The backend doesn't really care about things like templates, so the ordering of templates is one of the few non-trivial side-effects from the frontend that we care about.

From the backend perspective, there are two primary mechanisms that play into emission order:

- `getsym` pulls functions by IL offset, roughly the same order as the frontend, but with PCH interleaving
- `CgBottomUpOrder` iteratively emits every function whose callees have already been emitted

Inline asm `CALL`s do not count as callees in `CgBottomUpOrder`, so as a hack, a DCE'd branch (`if (0) callee();`) can be used to force deferral instead. (But don't use inline asm - go for matching.)

PCH functions keep their original IL offsets, and both the PCH and TU will start at offset zero, so their emission is interleaved with the TU itself, since `getsym` is sorting by IL offset. That means large includes in the PCH can impact interleaving. For reference, some known IL sizes: `<list>` is 22 KiB, `<string>` 40 KiB and `<math.h>` 11 KiB.

In some cases we're including `.inl` files at the _end_ of TUs, which causes callees to be deferred.

Static functions are omitted if nothing references them, but they are emitted as COMDAT if anything _does_ reference them, _even if they were always inlined_ and thus none of those references make it into the final image.

Some surprisingly simple things can lead to extra unmatched COMDAT symbols if they lead to different calls, for example, `(*it).x` vs `it->x`, or using `begin()` to get an iterator instead of default-initializing one directly.

Different ways of instantiating functions can sometimes cause emission order to change - explicit args vs forwarding overloads, direct vs indirect operator calls, etc. This may depend on *all* instantiations of a function in a translation unit, so some experimentation will be necessary.

## Inlining overview

VC 7.1 computes a cost for each call site using the callee's size and the call site's "weight", and rejects the call when the cost reaches 74 (`InlSpeedLimit`.) This heuristic can vary on details that emits identical code, like branches that get folded or DCE'd. For example, `if (a) return; if (b) return;` has a different weight than `if (a || b) return;` even though the emitted code may be identical.

## Scheduling overview

Blocks are scheduled top-down, with up to three instructions per cycle, one per functional unit. 

## Compare folding overview

`GoptConstantFoldCompareBlk` folds a chain of comparisons when the next comparison is reached through labels and
branches. `GoptCommonSubExpression` elides a re-test whose condition is known on every path entering the block. In some cases, statements that generate no code can prevent comparisons from being folded together, even as simple as unused local variable declarations.

## Loops overview

- Loop unrolling is sensitive to the type of the loop counter.
- Loops are unrolled _after_ CSE.
- `ptr[y * len + x]` can usually be rewritten as `ptr += len` by the compiler, but despite similar codegen behavior allocation will differ.

# Codegen troubleshooting

## Register allocation varies based on temporaries

Even though semantically, accessing a field via an inlined accessor is identical to just accessing the field directly, it will have an impact on how registers are allocated in MSVC.

For example,

```cc
WVideoDev* video = GetVideoDevice();
```

can be the same as

```cc
WResourceManager* resrc = m_resrcMng;
WVideoDev* video = resrc->video;
```

but

```cc
WVideoDev* video = m_resrcMng->video;
```

is _not_.

## Register allocation variation due to aliasing

Sometimes, a function will differ for reasons entirely outside of the function body itself; one such case is aliasing. A local is treated as aliased when:

- It is passed to an "opaque" function (one not defined in the current translation unit)
- It is passed to a function in which the pointer escapes
- It is stored to a field or global

It is _not_ aliased by a callee whose body _is_ visible and merely reads or writes through the pointer, even if the call is not inlined, regardless of linkage, insensitive to control flow and even if the code that does it is eliminated by DCE. Inlined helpers do not generally alias.

A struct copy into an aliased local results in load/store pairs in address order. A struct copy into an unaliased local does loads first, with the value consumed first hoisted to the front, and stores sinked next to their consumers. An aliased variable is not eligible for copy propagation, so it gets updated in-place plus a store, whereas unaliased variables are and get recomputed from the source.

The key takeaway here is that even if a function isn't inlined, its visibility to the current translation unit can have a profound impact on both register allocation and scheduling.

### Register allocation variation due to conditional assignment

When variables are initialized conditionally, it can look similar to an aliasing difference:

```cpp
WxVb* pVb = 0;
if (m_xpaVbs)
{
    pVb = m_xpaVbs;
    do
    {
        // ...
    } while (pVb);
}
```

Changing between `while` and `do while` has no impact here - it's just the conditional assignment at play. The trick here is that when doing this conditional assignment, it looks similar to an aliasing difference, as copy propagation doesn't occur like it would with an unconditional assignment. The same pattern can also impact register allocation and result in the allocation for `esi` and `edi` being swapped.

This can happen even if the conditional is eliminated from the code, which is probably why this one is so hard to spot in the first place. It is possibly a case where the developer did an unnecessary `if` guarding a loop.

### Floating point casts can impact scheduling without emitting instructions

Operands of commutative operations are sorted descending by `complexity << 16 | hash`. A memory operand's hash comes from its base temporary's ID. IDs are assigned in creation order. Named variables get IDs in the order they are referenced, parameters first, right-to-left. Inlined parameters have their own pool, also right to left. Hash variable IDs start at the next multiple of 0x20 after the last temporary. Float sums are flattened and sorted early, keyed on address expressions, then re-hashed and sorted twice more, keyed on hash-variable IDs. Anything that generates the same IL will also get the same operand order - the operand order is determined strictly by IL. That means in a matrix's union'd variables, `m11` is the same as `m[0][0]`, for example, since both generate the same IL.

#### Parentheses and casts

Parentheses and casts on floating point expressions are IL nodes and have an impact on the instruction scheduler even though they don't actually emit any code. That means that an extra pair of parenthesis in a helper function can shift the IDs and thus the instruction order of float ops in all of the callers.

With `/Op`, casts to floating point types can impact instruction scheduling counterintuitively. Even where a and b are `float`, `a + b` may result in different scheduling than `(float)(a + b)`. This effect applies within subexpressions, e.g. `(float)(c ? a + b : d - e)` is not equivalent to `c ? (float)(a + b) : (float)(d - e)`.

### Code generation variations caused by counterintuitive cost heuristics

- The compiler heuristics used to determine when to apply optimizations like inlining or loop unrolling often depends on seemingly superficial details, like no-op casts, temporaries, wrappers, etc.
- Equivalent ways of expressing loops often produce identical machine code but different cost heuristics, which can change whether a later call is eligible for inlining.

## General advice

MSVC 7.1 allocates registers lowest-free-first in the order they are created: `ecx`, `edx` then `eax`. Creation order is influenced by source order and not codegen order.

| Symptom                                                      | Cause                                            | Action                                                                 |
| ------------------------------------------------------------ | ------------------------------------------------ | ---------------------------------------------------------------------- |
| Struct copy is pairwise, should be loads-first/reordered     | Destination is aliased when it should not be     | Look for functions that are opaque and should not be                   |
| Value recomputed from another var instead of in-place update | Aliasing or conditional initialization elsewhere | Check opaque calls, potential DCE'd code, and local assignment paths   |
| Named local in ours doesn't exist in PDB                     | Var didn't exist/was narrow, wrong control flow  | Look for widening in PDB line table, convert `do/while` to `for`, etc. |
| Registers become mismatched at one point                     | Register allocation diverged due to temporaries  | Try re-ordering temporaries                                            |

Generally, as far as we can tell, the following things **usually do not impact code generation** (but sometimes do):

- &p[i] vs p + i
- Regrouping values (distributive/associativity/commutative)
- De Morgan's law/flipping conditional blocks
- Ternary vs if/else conditional
- `const` and non-`const` locals
- Different ways of writing `operator=`
- chained vs separate assignment expressions

The following things **often impact code generation**:

- Visibility of non-inlined inline function bodies
- The actual inlined body, e.g. one-return ternary vs two-return if/else
- Types of different widths
- Whether a value is a distinct variable or not
- Conditional initialization of a list cursor, even when the conditional disappears from the emitted code
- FP products (without `/Op`): `fld` gets a fresh memory operand, `fmul` gets operand with known value from earlier use. If both are fresh, higher offset gets `fld`. Sums with 3 terms are re-associated, descending. Source operand order doesn't matter, but expression structure does - scalar temporaries vs `WVector` will impact frame size.

Remember to confirm the full image diff when working on things, not just per-symbol diffs, since some changes can have far-reaching consequences. (If the image diff becomes huge, it most certainly means it has become misaligned.) Once you've found a promising lead, remember to try removing any hacks that were present to try to work around a mismatch, lest you get stuck forever in a local maxima.

# Reference the PDB

The PDB often offers insight that can be used to narrow the scope of possible matching source codes. (Note that you will have to use the whole-image PDB - the delinked objects don't contain any debug information by their nature.) It can be wise to _start_ with the PDB information _first_ when decompiling, rather than work backwards into it.

Since these PDBs are old, you can't use `llvm-pdbutil` or most other tools to read them. IDA Pro handles them fairly well, and Ghidra also handles them reasonably well, but neither of those tools allow access to _all_ of the useful information of the PDB (to my knowledge.) A copy of Microsoft's useful `cvdump` utility is included, for convenience. It isn't necessarily easy to use, but it is the best option available as far as I know. Check [the corresponding README](../tools/cvdump/README.md) for information on how to use `cvdump` in this project.

For convenience, there is also a wrapper around cvdump, [pdbinfo.py](../tools/pdbinfo.py). It is just for convenience and doesn't really produce anything cvdump could not, but since it's easier to use it should probably be reached for first.

Note that this only applies to units that actually contain useful PDB information. Unfortunately, most (all?) of the ProjectG units don't, so these techniques can't be used for those.

## Line tables

- Stored in each module stream after the symbols. Offsets are section-relative.
- Align them with the disassembly to deduce:
  - Early returns vs nesting
  - Ternary expressions, chained assignments, or comma-joined assignments
  - Gaps with no code
  - Which `if`/`else` block should be first in source code
  - `for` loops, where loop post-expression is attributed to the `for` line
  - Which statement a widening belongs to, to deduce a variable's type
- Limitations:
  - Only a statement's first instruction is recorded
  - Instructions migrated to another block aren't present
  - Missing records don't prove anything
- Differential analysis of our PDB vs the reference can be enlightening.

## Local records

- `S_BPREL32` records are emitted for each scope, outer scope first, ordered depending on names.
- If we match our locals to the PDB names and declare them all in one scope, we can deduce which locals originally shared a scope.
- Locals backed by a register do not have a record.
- A stack record does not prove the variable was aliased; it could just be a result of register pressure. Lack of copy propagation is more compelling evidence.
- Parameters and locals can share stack offsets; incorrectly initializing a local that was originally not initialized, or providing a default argument where there wasn't one, can impact this behavior and impact when leaf nodes are merged.

## Zero-initialized globals

Uninitialized declarations and explicit zero initializers for globals have different behaviors with regards to the order and padding of globals. This applies regardless of whether or not the data goes to `.bss`. Check PDB for hints.

## Header inlines

- A function whose line record points into a header file (`.h` or `.inl`) is a header inline.
- As a result of COMDAT `any` selection, an arbitrary object will be the contributor.
- Some units have different compiler flags, e.g. some have `/Op` and some do not. When matching the implementation of an inline function, find the unit that it belongs to. The `tools/address2sym.py` script can be used here.

# Image mismatch due to out-of-line inlines

Even when every inline call to a function in a given unit is indeed inlined, in some cases it will still result in an out-of-line COMDAT copy. The final image is linked with `/DEBUG`, which prevents the unreferenced copy from being stripped. The linker will remove the duplicate copies of these inlined functions across all of the objects, leaving only one: whatever the first copy that was contributed was.

Because these inline functions are otherwise unreferenced, they usually don't have a name in the debug infomration. Because of that, they have a synthesized name instead, in the form of `__pg_c_*`. Because COMDAT deduplication is keyed off of the symbol name, these synthesized names no longer get deduplicated as expected once we start swapping some objects out for source code, resulting in an image mismatch.

The fix is to rename the symbol once it has been identified:

```shell
$ python tools/coffsym.py rename __pg_c_001786 '?GetVideoDevice@WView@@QBEPAVWVideoDev@@XZ'
$ python tools/coffsym.py set-selection ./source/client/ProjectG/pool.obj '?GetVideoDevice@WView@@QBEPAVWVideoDev@@XZ' any
```

The `rename` tool will rename the symbol across the entire tree. This is necessary so that external references can be correctly resolved.

In a lot of cases, you can figure out what the symbol is supposed to be by looking for clues in the PDB. If that fails you, then compile with the `/FAcs` flags. It will give you an assembly listing with data and code intermixed. Then, search for a match at a plausible symbol address. Since the relocated operands will be unresolved in the output, they need to be wildcarded. Another challenge is the fact that not all translation units were compiled with the same flags, so sometimes the copy that makes it into the image was built with different compiler flags than the one you're currently looking at. You may have to cycle through different compiler flags, such as `/Op` or `/Oy`.

There may be multiple chunks that match exactly. Unfortunately, it isn't really possible to be 100% sure which chunk corresponds to what you are looking at. Do not rename everything that is a perfect match; only one symbol can possibly be the _actual_ match.

Putting `__declspec(dllimport)` on an inline function will actually stop the out-of-line copy from being emitted at all, but this is a dirty workaround for the problem. If plausible, it's better to try to find the original copy. In some cases, it may not be trivial.

# COMDAT emission order follows code generation order

The linker outputs sections in the order the compiler emits them, and the compiler emits inline bodies as soon as it finishes code generation for them. Therefore, restructuring a class can change the image identity.

# Trace the backend

If all else fails, you can debug the compiler backend itself to figure out what's going on. `tools/c2trace.py` runs the compilation of a single unit with breakpoints added for instrumentation. You can insert breakpoints anywhere, but here are some useful candidates:

| Function | Address | Use |
| --- | --- | --- |
| `ShouldInlineCallTuple` | 10760c81 | Inlining decisions per call |
| `InlCallGraphDecision` | 10760ac1 | Inlining decisions per call site |
| `OptCmpHashVal` | 10795bc5 | Commutative operand/FP sum order |
| `MscMergeSort` | 10795adb | Operand order sort |
| `getsym` | 1074f85c | Function list construction |
| `CgBuildCallGraph` | 1070c93c | Call graph (used for emission order) |
| `CgBottomUpOrder` | 1070d05b | Function emission wave |
| `GlobalOptimize` | 10758bb3 | Global optimizer |
| `GoptConstantFoldCompareBlk` | 1074b29d | Compare chain folding logic |
| `GoptCommonSubExpression` | 1074d6a8 | CSE opt pass |
| `OrderGlobregs` | 107ac06a | Register ordering |
| `UpdateAffectedGlobregs` | 107aa17f | Split rounds during reg alloc |
| `FPSpillWorthless` | 10710c46 | Freed register ranges |
| `LoptBuildAddressModes` | 1076e33e | Address mode (`lea`, SIB operand) formation |
| `AvoidAirlocks` | 107a711b | Extra stack copies |
| `DagBuildGraph` | 107addc0 | Instruction scheduler dependence graph |
| `Schedule`, `SchSchedGraph` | 107b1009, 107b0b9b | Instruction scheduling |

At each breakpoint, the tracer prints out a dump of the registers and stack; the actual values are not interpreted, so you'll need a disassembly of the relevant function (via Ghidra or IDA) to follow along with the logic. The c2trace program is hardcoded for tracing the backend for now.
