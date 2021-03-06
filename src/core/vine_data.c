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


#include "vine_data.h"
#include "vine_pipe.h"

vine_data_s* vine_data_init(vine_pipe_s * vpipe, size_t size,
                            vine_data_alloc_place_e place)
{
	vine_data_s *data;

	/* Not valid place */
	if(!place || place>>2)
		return 0;


	data = (vine_data_s*)vine_object_register(&(vpipe->objs),
											  VINE_TYPE_DATA,
										   "",size+sizeof(vine_data_s));

	if(!data)
		return 0;

	data->place = place;
	data->size  = size;
	data->flags = 0;
	async_completion_init(&(vpipe->async),&(data->ready));
	return data;
}

size_t vine_data_size(vine_data *data)
{
	vine_data_s *vdata;

	vdata = data;
	return vdata->size;
}

void* vine_data_deref(vine_data *data)
{
	vine_data_s *vdata;

	vdata = offset_to_pointer(vine_data_s*, vpipe, data);

	if (!(vdata->place&HostOnly)) {
		return 0;
	}

	return (void*)(vdata+1);
}

void vine_data_mark_ready(vine_pipe_s *vpipe, vine_data *data)
{
	vine_data_s *vdata;

	vdata = offset_to_pointer(vine_data_s*, vpipe, data);
	async_completion_complete(&(vdata->ready));
}

int vine_data_check_ready(vine_pipe_s *vpipe, vine_data *data)
{
	vine_data_s *vdata;
	int return_val;

	vdata = offset_to_pointer(vine_data_s*, vpipe, data);
	return_val = async_completion_check(&(vdata->ready));

	return return_val;
}

void vine_data_free(vine_pipe_s *vpipe, vine_data *data)
{
	vine_data_s *vdata;

	vdata = offset_to_pointer(vine_data_s*, vpipe, data);
	vine_object_ref_dec(&(vdata->obj));
}

int vine_data_valid(vine_object_repo_s *repo, vine_data_s *data)
{
	return 0;
}

VINE_OBJ_DTOR_DECL(vine_data_s)
{
	vine_data_s * data = (vine_data_s *)obj;
	arch_alloc_free(obj->repo->alloc,data);
}
