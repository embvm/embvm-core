# 13. Use Templates to Switch Between Dynamic & Static Constructs

Date: 2018-11-30

## Status

Accepted

## Context

We want to build the framework components to work with both dynamic and static memory allocation schemes.

## Decision

Rather than duplicating implementations, we will use Template Metaprogramming to use a single structure which supports both static and dynamic memory allocation. Classes can be templated with a size parameter. This size is evaluated to determine the underlying data structure that will be used. If the size is equal to 0, dynamic memory is assumed. Otherwise, static memory allocation will be used.

## Consequences

These modules become much more complicated to understand, as they rely on template metaprogramming. Many developers are leery of templates and have a hard time understanding the code.
