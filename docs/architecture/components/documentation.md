# CRC-R: Framework Documentation

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Provides users with references for working with the framework
* Provides examples for using the framework
* Provides rationales for all decisions made regarding the framework

## Requirements

* Documentation should be search-able
* Documentation should be contained alongside source-code

## Collaborators

* Generated by the [Build System](build_system.md)

## Rationale

A project is not useful without documentation.

Keeping the documentation in revision control helps track changes and enables reviews and process controls.

Keeping the documentation alongside source code improves the chances it will be referred to and updated.

## Source Links

Documentation lives throughout the project.

Each header file needs to contain its own API documentation.

## Notes

On including pattern references:

> Mature frameworks usually incorporate several design patterns. The patterns help make the framework’s architecture suitable to many different applications without redesign. An added benefit comes when the framework is documented with the design patterns it uses [BJ94]. People who know the patterns gain insight into the framework faster. Even people who don’t know the patterns can benefit from the structure they lend to the framework’s documentation. Enhancing documentation is important for all types of software, but it’s particularly important for frameworks. Frameworks often pose a steep learning curve that must be overcome before they’re useful. While design patterns might not flatten the learning curve entirely, they can make it less steep by making key elements of the framework’s design more explicit.
