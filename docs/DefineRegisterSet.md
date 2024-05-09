# Define the Register Set of your target
> **Note:** This document applies to LLVM-17.0.x version. The information provided here might not be suitable for other LLVM versions. 

The register set definitions are done in so called `TableGen files` with the extension `.td`. `TableGen` will generate `C++` classes and methods based on the information defined in the `td` files. These definitions will be included in `C++` and makes development much easier, as it takes care of a lot of boiler plate code.

To define the Register Set of your target in `LLVM`, you need to write the `SkeletonRegisterInfo.td` file according to the [LLVM Documentation](https://llvm.org/docs/WritingAnLLVMBackend.html#register-set-and-register-classes). This document will guide you through the multiple steps involved in this process.

> **Note** Replace `Skeleton` with the name of your target in all the sections of this document. The target name should be capitalized and camel-case if it's composed of multiple words.

## Create a Register Class that defines one register
```
class SkeletonReg<bits<5> Enc, string n> : Register<n> {
  let HWEncoding{15-5} = 0;
  let HWEncoding{4-0} = Enc;
  let Namespace = "SKELETON";
}
```
In this case, the registers are identified with 5-bit ID numbers. Please change the encoding according to your target specification. `TableGen` will generate a `C++` enum with the `Namespace` name. This enum will contain all the registers of this target. There are multiple properties you can define for a `Register`.

> **Note** To see all the properties you can define for a `Register`, please refer to the `Register` class definition in `llvm/include/llvm/Target/Target.td`.

## Create registers
`TableGen` provides an easy way of defining multiple objects of the same type by using a for loop as shown below, where we are definind 32 registers of type `SkeletonRegister` (which we defined in the [previous section](#create-a-register-class-that-defines-one-register)).

```
// Data registers
foreach i = 0-31 in {
  def R#i : SkeletonReg<i, "R"#i>, DwarfRegNum<[i]>;
}
```
In this example we define integer registers with names `R0-R31`. `DwarfRegNum` is a debugging identifier for the register. We can, of course, also define address registers in a similar manner.

```
// Address registers
foreach i = 0-31 in {
  def A#i : SkeletonReg<i, "A"#i>, DwarfRegNum<[i]>;
}
```

## Create a Register Class that holds all related registers
Until now we only defined the names of the registers and their encodings, however, we don't have any information about their bit-width or their types. We do that by creating a `Register Class` that holds registers of the same type defined in the [previous section](#create-registers). 

### First we need to define the `RegisterClass`
```
// Register classes.
class SkeletonRegisterClass<list<ValueType> types, int size,
                          int alignment, dag regList, int copyCost = 1>
    : RegisterClass <"SKELETON", types, alignment, regList> {
      let Size = size;
      let CopyCost = copyCost;
}
```
| Argument      | Description |
| ------------- | ------------- |
| `types`       | The type of values that the registers can hold  |
| `size`        | The size of the registers in bits  |
| `alignment`   | The alignment of the registers  |
| `regList`     | A directed acyclic graph (DAG) that represents the list of registers in the register class  |
| `copyCost`    | The cost of copying the register  |  

Feel free to define as many of these classes as needed.
> **Note** To see all properties you can define for a `RegisterClass`, please refer to the `RegisterClass` class definition in `llvm/include/llvm/Target/Target.td`.

### Then we need to instantiate `SkeletonRegisterClass`
Once we have the register class defined, we can create groupings of registers. For instance, we can group all the data registers together in a class called `GPR`, or `General Purpose Registers`, as shown below.

```
def GPR : SkeletonRegisterClass <[i32], 32, 32, (add (sequence "R%u", 0, 31))>;
def AddressRegs : SkeletonRegisterClass <[iPTR], 32, 32, (add (sequence "A%u", 0, 31))>;
```
As can be seen above, `GPR` Register class holds the `R0-R31` registers, which are 32-bit wide and can hold 32 bit integer values. The `AddressRegs` Register class holds the address registers that hold 32-bit pointers.

> **Warning** The order in which we add the registers in this class matters. The **order defines the priority** of each register in the register allocation algorithm later on. For this reason, it is common practice to **add all the registers manually in a list**. However, if there is no special order defined by the target specification, you can add all the registers as shown above. This will add them in order from `R0` to `R31`, with `R0` being the first register to be allocated.

## Create register operands
In LLVM, a register operand is a reference to a register that is used as an operand in an instruction. The process to create a register operand is somewhat similar to the [register class](#create-a-register-class-that-holds-all-related-registers). 

### Write register operands class
```
// Register operands
class SkeletonRegisterOperand<RegisterClass RC> : RegisterOperand<RC>
{
  let DecoderMethod = "decode"#RC#"RegisterClass";
}
```
The `DecoderMethod` is a property of a `RegisterOperand` that specifies the method used to decode that operand from machine code during disassembly.This means that when a `SkeletonRegisterOperand<GPR>` is being disassembled, the `decodeGPRRegisterClass` method will be used to decode it from machine code back into a `SkeletonRegisterOperand<GPR>`.

### Instantiate the `SkeletonRegisterOperand` class
```
def GPROperand : SkeletonRegisterOperand<GPR>
{
  let GIZeroRegister = R0;
}
```
Here, we are creating the actual register operand class. This will reference all registers that are held by the `GPR` register class we [created before](#create-a-register-class-that-holds-all-related-registers). `GIZeroRegister` specifies which register always holds the value 0, if the target supports that. 

> **Note** To see all properties you can define for a `RegisterOperand`, please refer to the `RegisterOperand` class definition in `llvm/include/llvm/Target/Target.td`.
