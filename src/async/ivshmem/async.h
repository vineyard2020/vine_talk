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


#ifndef ASYNC_HEADER
#define ASYNC_HEADER
#include "core/vine_object.h"
#include "utils/list.h"
#include "utils/spinlock.h"
#include "pthread.h"
#include "arch/alloc.h"

typedef struct ivshmem ivshmem_s;

typedef struct async_meta_s
{
	utils_spinlock lock;
	utils_list_s outstanding;
	ivshmem_s * regs;
	arch_alloc_s * alloc;
	pthread_t thread;
	volatile int fd;
}async_meta_s;

typedef struct async_completion_s
{
	async_meta_s * meta;
	utils_list_node_s outstanding;
	size_t vm_id;
	volatile size_t completed;
	pthread_mutex_t mutex;
	pthread_mutexattr_t attr;
}async_completion_s;

typedef struct
{
	async_meta_s * meta;
	utils_list_s pending_list;
	utils_spinlock pending_lock;
	volatile size_t value;
}
async_semaphore_s;

typedef struct
{
	async_meta_s * meta;
	async_completion_s mutex;
	async_semaphore_s semaphore;
}async_condition_s;

#include "async_api.h"
#endif
