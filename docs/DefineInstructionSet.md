# Define the Instruction Set of your target
> **Note:** This document applies to LLVM-17.0.x version. The information provided here might not be suitable for other LLVM versions. 

The instruction set definitions are done in so called `TableGen files` with the extension `.td`. `TableGen` will generate `C++` classes and methods based on the information defined in the `td` files. These definitions will be included in `C++` and makes development much easier, as it takes care of a lot of boiler plate code.

In general, the process is quite straight-forward as it implies defining the `Instruction Formats`, defining special `operands`, `patterns` and also the instructions themselves. The instruction set definitions sits in the `SkeletonInstrFormats.td` and `SkeletonInstrInfo.td`.

> **Note** Replace `Skeleton` with the name of your target in all the sections of this document. The target name should be capitalized and camel-case if it's composed of multiple words.

## Create the base instruction format class
In order to start writing your own instruction formats, you need to define a base class for all these formats/instructions that will inherit from it. This is not mandatory but it is highly recommended as it gives a lot of flexibility when defining instruction formats specific to the target.

In the code block below there is an example of such a class, however, please keep in mind that this class is highly dependant on your specific architecture.

This class definition should be placed in `SkeletonInstrFormats.td`.

```
class SkeletonInst<dag outs, dag ins, string asm, list<dag> pattern = []>
    : Instruction {
  bits<32> Inst;
  let Namespace = "SKELETON";
  let Size = 4;
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asm;
  let Pattern = pattern;
  let DecoderNamespace = "SKELETON";
}
```
### Arguments
| Argument  | Description |
| ------------- | ------------- |
| `outs`  | A directed acyclic graph (DAG) representing the output operands of the instruction |
| `ins`  | A DAG representing the input operands of the instruction |
| `asm`  | A string representing the assembly syntax for the instruction |
| `pattern`  | A list of DAGs representing the pattern for the instruction (optional, default is an empty list) |
___

### Class properties

| Property  | Description |
| ------------- | ------------- |
| `Inst`  | A 32-bit field representing the instruction. Please define the bit-width of the instructions according to your target's specs |
| `Namespace`  | A string value used to indentify the instructions in C++ |
| `Size`  | Size of the instruction in bytes |
| `OutOperandList`  | A DAG representing the output operands of the instruction |
| `InOperandList`  | A DAG representing the input operands of the instruction |
| `AsmString`  | A string representing the assembly syntax for the instruction |
| `Pattern`  | A list of DAGs representing the pattern for the instruction |
| `DecoderNamespace`  | A string set to "SKELETON", representing the namespace for the decoder |
___
> **Note** There are several propoerties that one can set here. To see all the properties you can define for a `Instruction`, please refer to the `Instruction` class definition in `llvm/include/llvm/Target/Target.td`.

## Define Instruction Formats

Once the base class is defined, you have the flexibility to define the instruction formats based on your target's specifications. Typically, instructions in the ISA are categorized by operand types or instruction types. For instance, algorithmic/logical/memory instructions or instructions that operate on registers only or registers and immediate values. There is no definitive approach to this, and it largely depends on your specific requirements.

In official LLVM backends, it is typical to have multiple layers of hierarchy in instruction formats. This decision can also be based on personal preference.

For instance, you may have instructions that has as output a register and as input another register, as the example below:
```
// Category: Integer.
class F_I<dag outs, dag ins, string asm, list<dag> pattern = []>
   : SkeletonInst<outs, ins, asm, pattern> {
  bits<5>  rd;
  bits<5>  rs1;
  let Inst{25-21} = rd;
  let Inst{20-16} = rs1;
}
```
In this example, we have the format named `F_I`, which inherits from `SkeletonInst`, but adds two more fields that will be encoded in the `Inst` field from the parent class. This format will be the basis for other specializations such as register-register or register-immediate formats. 
For other examples of instruction formats please refer to [the Toy architecture implementation](../Prototype/PrototypeInstrFormats.td). 

Every target architecture has the instruction formats defined such that they apply to the specifics of that target. Thus, this not an exact science. 

> **Note** Please refer to [this document](DefineRegisterSet.md) for details on how to define the register set. 

## Define instructions
Once we have a few formats defined, we can start writing instruction definitions. 
