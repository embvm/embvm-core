#ifndef COMPILER_DEFS_H_
#define COMPILER_DEFS_H_

// TODO: move this somewhere safe to do a system header include to squash warnings

#define __printf_check(istr, iarg) __attribute__((format(printf, istr, iarg)))
#define __printf_check_cpp(istr, iarg) __attribute__((format(printf, istr + 1, iarg + 1)))

#endif // COMPILER_DEFS_H_
