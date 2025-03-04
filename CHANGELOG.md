# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-03-04
First major release of this project.

### Added
- Introduced a minimal LLVM backend template named `Skeleton`.
- Developed helper scripts using Python `invoke` tasks to automate the creation and building of a new LLVM Backend Development Environment. The specified LLVM release is automatically downloaded and extracted to the desired location.These scripts then ensure that the custom backend is fully integrated into the LLVM, CLANG, and LLD build systems, streamlining the initial setup process.
  - Added `setup-new-environment` task to set up a new LLVM Backend Development Environment.
  - Added `create-target-in-environment` task to add a new target to an existing environment.
	- Added `build` task to trigger the build process for the whole environment.
- Added support for LLVM 17 (version 17.0.6).
### Changed

### Removed
