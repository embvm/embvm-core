// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

/// Operating system types and definitions
namespace os
{
/** @defgroup FrameworkOSImpl Supported Framework Operating Systems
 *
 * Operating system impelmentations designed to work with the framework virtual machine.
 */

/// POSIX types and definitions for OSX and Linux
namespace posix
{
/** @defgroup POSIXOS POSIX OS Types
 *
 * Virtual OS implementation for POSIX systems.
 *
 * @ingroup FrameworkOSImpl
 */
};

/// Unit test operating system
namespace test
{
/** @defgroup UnitTestOS Unit Test OS types
 *
 * OS types used for unit testing.
 *
 * @ingroup FrameworkOSImpl
 */
};

} // namespace os
