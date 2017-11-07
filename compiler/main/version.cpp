/*
 * Copyright 2004-2017 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdio>
#include "driver.h"
#include "version.h"

// this include sets BUILD_VERSION
#include "version_num.h"

// this include sets CONFIGURED_PREFIX
#include "configured_prefix.h"

// this include sets CLANG_SETTINGS
#include "clang_settings.h"

void
get_version(char *v) {
  v += sprintf(v, "%d.%s.%s", MAJOR_VERSION, MINOR_VERSION, UPDATE_VERSION);
  if (strcmp(BUILD_VERSION, "0") != 0 || developer)
    sprintf(v, " pre-release (%s)", BUILD_VERSION);
}

void
get_major_minor_version(char *v) {
  sprintf(v, "%d.%s", MAJOR_VERSION, MINOR_VERSION);
}

const char*
get_configured_prefix() {
  return CONFIGURED_PREFIX;
}

const char*
get_clang_cc() {
  return CLANG_SETTINGS[0];
}

const char*
get_clang_cxx() {
  return CLANG_SETTINGS[1];
}

const char*
get_clang_sysroot_args() {
  return CLANG_SETTINGS[2];
}
