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


#ifndef VINE_VACCEL_HEADER
#define VINE_VACCEL_HEADER
#include "vine_talk_types.h"
#include "utils/queue.h"
#include "core/vine_object.h"

typedef struct vine_vaccel_s vine_vaccel_s;

#include "core/vine_accel.h"

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

/**
 * Virtual Accelerator
 *
 * Creates a dedicated queue mapped to a physical accelerator.
 */
struct vine_vaccel_s {
	vine_object_s     obj;
	vine_accel_type_e type;
	async_condition_s cond_done;	// Condition notifying task completion
	uint64_t          task_done;	// Counter of completed tasks.
	utils_list_node_s vaccels;
	utils_spinlock    lock;
	uint64_t          cid;
	uint64_t          priority;
	vine_accel_s      *phys;
	void              *meta;	// Metadata pointer available to controller.
	utils_queue_s     queue;
};

/**
 * Initialize a vine_vaccel_s in \c mem.
 *
 * \param repo A valid vine_object_repo_s instance
 * \param name Name of the virtual accelerator
 * \param type Type of the virtual accelerator
 * \param accel A physical accelerator
 */
vine_vaccel_s* vine_vaccel_init(vine_pipe_s * pipe, const char *name,
								vine_accel_type_e  type,vine_accel_s *accel);

/**
 * Set the client id for this virtual accelerator.
 */
uint64_t vine_vaccel_set_cid(vine_vaccel_s *vaccel,uint64_t cid);

/**
 * Get the client id for this virtual accelerator.
 */
uint64_t vine_vaccel_get_cid(vine_vaccel_s *vaccel);

/**
 * Set the priority (latency or throughput critical) for this virtual accelerator.
 */
uint64_t vine_vaccel_set_job_priority(vine_vaccel_s *vaccel,uint64_t priority);

/**
 * Get the priority (latency or throughput critical) for this virtual accelerator.
 */
uint64_t vine_vaccel_get_job_priority(vine_vaccel_s *vaccel);

/**
 * Get the meta for this virtual accelerator.
 */
void vine_vaccel_set_meta(vine_vaccel_s *vaccel,void * meta);

/**
 * Set the meta for this virtual accelerator.
 */
void * vine_vaccel_get_meta(vine_vaccel_s *vaccel);

/**
 * Get the queue of \c vaccel.
 *
 * \param vaccel A virtual accelerator
 * \return The queue of \c vaccel,NULL on failure
 */
utils_queue_s* vine_vaccel_queue(vine_vaccel_s *vaccel);

/**
 * Requrn size of \c vaccel.
 *
 * \param vaccel A virtual accelerator
 * \return The size of the queue of \c vaccel.
 */
unsigned int vine_vaccel_queue_size(vine_vaccel_s *vaccel);

vine_accel_state_e vine_vaccel_get_stat(vine_vaccel_s *accel,vine_accel_stats_s * stat);

/**
 * Block until atleast one task issued to \c accel is done.
 *
 * \Note 'Done' in this case includes Successful or Failed tasks.
 */
void vine_vaccel_wait_task_done(vine_vaccel_s *accel);

/**
 * Notify and unblock any/all processes or threads blocked at a
 * vine_vaccel_wait_task_done(\c accel) invocation.
 *
 * \Note 'Done' in this case includes Successful or Failed tasks.
 */
void vine_vaccel_mark_task_done(vine_vaccel_s *accel);

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef VINE_VACCEL_HEADER */
