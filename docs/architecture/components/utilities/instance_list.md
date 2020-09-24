# CRC-R: Instance List

A utility which enables you to keep track of pointers of a specific type, such as Driver instances.

**Status:** Completed

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Keeps track of pointers to objects of a specific type

## Requirements

* Provides ability to add or remove objects from list at runtime
* Provides ability to search through the list for a specific object
* Works with a wide variety of STL and ETL containers
* Provide both static and dynamic memory allocation schemes
* Return references to objects, instead of pointers

## Collaborators

* Instance List is used by the [Driver Registry](../core/driver_registry.md) to keep track of [Driver](../core/driver.md) objects.

## Rationale

This construct helps keep track of instances of objects in a manner that supports both static and dynamic memory allocation schemes.

References are returned instead of pointers, eliminating the possibility of nullptr dereference

## Source Links

* [Instance List Folder](../../../../src/utilities/instance_list)
* Instance List
	* [instance_list_test.cpp](../../../../src/utilities/instance_list/instance_list_test.cpp)
	* [instance_list.hpp](../../../../src/utilities/instance_list/instance_list.hpp)

## Related Documents

N/A

## Notes

N/A
