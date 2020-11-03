// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef COMPILER_DEFS_H_
#define COMPILER_DEFS_H_

// TODO: move this somewhere safe to do a system header include to squash warnings

#define __printf_check(istr, iarg) __attribute__((format(printf, istr, iarg)))
#define __printf_check_cpp(istr, iarg) __attribute__((format(printf, istr + 1, iarg + 1)))

#endif // COMPILER_DEFS_H_
