add_llvm_component_library(LLVMSkeletonDesc
  SkeletonAsmBackend.cpp
  SkeletonELFObjectWriter.cpp
  SkeletonInstPrinter.cpp
  SkeletonMCAsmInfo.cpp
  # SkeletonMCCodeEmitter.cpp
  SkeletonMCExpr.cpp
  SkeletonMCTargetDesc.cpp

  LINK_COMPONENTS
  SkeletonInfo
  MC
  MCDisassembler
  Support
  TargetParser

  ADD_TO_COMPONENT
  Skeleton
)
