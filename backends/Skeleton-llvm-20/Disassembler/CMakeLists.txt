add_llvm_component_library(LLVMSkeletonDisassembler
  SkeletonDisassembler.cpp

  LINK_COMPONENTS
  CodeGenTypes
  SkeletonDesc
  SkeletonInfo
  MC
  MCDisassembler
  Support

  ADD_TO_COMPONENT
  Skeleton
  )
