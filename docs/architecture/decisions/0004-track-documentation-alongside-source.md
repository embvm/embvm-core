# 4. Track Documentation Alongside Source

Date: 2018-07-06

## Status

Accepted

## Context

Project documentation is:

* Usually tracked in a variety of locations (google drive, various hard drives, wiki, repository)
* Rarely maintained by an organization
* Not updated to match source changes
* Copied multiple times, often with differences between copies/locations
* Usually not given a responsible party to own it

As a result, documentation is scattered and rarely kept up to date. If it is used, documenation is often generated and referred to once.

Quality documentation, especially when kept up-to-date, enables us to support more clients and developers with less direct involvement.

## Decision

All architectural, design, development, and user documentation will be maintained in the same repository as the source code.

This means that documentation should be generated when possible, rather than manually constructed in a Microsoft Word document.

## Consequences

* Documentation will have a higher change of remaining up-to-date (given the code documentation is included)
* Documentation must be markdown formatted or Doxygen parseable
* Images and PDFs will need to be tracked within the repository using git-lfs
* Documentors must work within the git repository
* Documentation changes are now eligible for review & PR processes
* It is easy to enforce documentation updates with code changes during the review process
* Checklists can be created to update documentation when code is changed
* If a modeling tool is used, XMI and image output should be tracked inside of the repository
