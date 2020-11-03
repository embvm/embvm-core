// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifdef __cplusplus
extern "C" {
#endif

const char* version_str();
const char* version_str_short();
const char* build_id(); // commit
const char* build_machine();
const char* build_timestamp();
const char* build_os();

#ifdef __cplusplus
}
#endif
