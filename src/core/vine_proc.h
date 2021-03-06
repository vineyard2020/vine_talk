/*
 * Copyright 2018 Foundation for Research and Technology - Hellas
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0 [1] [1]
 *
 * Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 *  implied.
 * See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 * Links:
 *  ------
 * [1] http://www.apache.org/licenses/LICENSE-2.0 [1]
*/


#ifndef VINE_PROC_HEADER
#define VINE_PROC_HEADER
#include <vine_talk.h>
#include "core/vine_object.h"
#include "utils/breakdown.h"

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

typedef struct {
	vine_object_s     obj;
	vine_accel_type_e type;
	int               users;
	size_t            bin_size; /**< binary size in bytes */
	utils_breakdown_stats_s breakdown;
	/* To add more as needed */
} vine_proc_s;

/**
 * Initialize a vine_proc at the memory pointed by \c mem.
 *
 * @param repo The vine_object_repo_s that will track the initialied procedure.
 * @param name NULL terminated string, will be copied to provate buffer.
 * @param type Accelerator type.
 * @param code Pointer to bytes containing procedure executable.
 * @param code_size Size of \c code parameter
 * @return An initialized instance of vine_proc_s, NULL on failure.
 */
vine_proc_s* vine_proc_init(vine_object_repo_s *repo, const char *name,
							vine_accel_type_e type, const void *code,
							size_t code_size);

/**
 * Compare \c code with the \c proc binary.
 *
 * @param proc Initialized vine_proc instance.
 * @param code pointer to binary code.
 * @param code_size \c length in bytes.
 * @return If the bytecodes match return 1, otherwise return 0.
 */
int vine_proc_match_code(vine_proc_s *proc, const void *code, size_t code_size);

/**
 * Get pointer to bytecode and size of bytecode for \c proc.
 *
 * @param proc An initialized vine_proc_s instance.
 * @param code_size Set to the codes size in byte.
 * @return Pointer to bytecode.
 */
void* vine_proc_get_code(vine_proc_s *proc, size_t *code_size);

/**
 * Modify user counter of \c proc.
 *
 * users += \c delta
 *
 * @param proc An initialized vine_proc_s instance.
 * @param delta Increase/decrease users by this amount.
 * @return The value of user after the modification.
 */
int vine_proc_mod_users(vine_proc_s *proc, int delta);

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef VINE_PROC_HEADER */
