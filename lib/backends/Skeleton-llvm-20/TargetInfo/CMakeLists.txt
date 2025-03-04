add_llvm_component_library(LLVMSkeletonInfo
  SkeletonTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  Skeleton
  )
