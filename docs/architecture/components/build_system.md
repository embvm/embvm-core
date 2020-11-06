# CRC-R: Build System

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Compile framework and client application
* Compile and run unit tests
* Generate documentation
* Provide hooks for quality control measures such as code formatting and static analysis

## Requirements

* Supports cross-compilation
* Allow users to configure settings
* Allow users to select what modules to build
* Allow users to define multiple platforms and applications
* Needs to be easy to build a subset of software (e.g., for host testing)

## Collaborators

* Builds the [Documentation](documentation.md)

## Rationale

Without a build system, the project can't be distributed or used.

The build system must be flexible and straightforward to use/configure.

## Source Links

The build system exists throughout the project:

* The root [meson.build](../../../meson.build) file is master build system file and invokes subdirectory build files
* The [build/](../../../build/) folder is a collaborating set of files used by the build, including cross-compilation support and toolchain settings
* [Jenkinsfile](../../../tools/Jenkinsfile) and [CI.jenkinsfile](../../../tools/CI.jenkinsfile) contain the build rules for the Jenkins build server

## Related Documents

N/A

## Notes

* Want to keep the client build separated from the core build
* Can we utilize a module concept for some of our packages? C++/Conan modules?
	* Look into [Meson Subprojects](http://mesonbuild.com/Subprojects.html)
* Look into [`meson configure`](http://mesonbuild.com/Configuring-a-build-directory.html) command, [build options](http://mesonbuild.com/Build-options.html), and [configuration options](http://mesonbuild.com/Configuration.html).
