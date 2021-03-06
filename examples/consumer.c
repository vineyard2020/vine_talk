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


#include <vine_pipe.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


volatile int run = 1;
static void quit(int sig)
{
	run = 0;
}

int main(int argc, char *argv[])
{
	vine_pipe_s     *vpipe;
	vine_task_msg_s *msg;

	if (signal(SIGINT, quit) == SIG_ERR) {
		fputs("Could not set the signal handler.\n", stderr);
		return -1;
	}

	vpipe = vine_talk_init();

	if (!vpipe) {
		printf("VineTalk initialization failed!\n");
		return -1;
	}



	printf("Consumer started.\n");
	while (run) {
		printf( "Used:%d\n", utils_queue_used_slots(
						vpipe->queue) );
		msg = (vine_task_msg_s*)utils_queue_pop(vpipe->queue);
		if (msg) {
			printf("Got task (%p) %s(%s)!", msg, vine_accel_get_name(
			               msg->accel),
			       ( (vine_proc_s*)msg->proc )->obj.name);

			sprintf( (char*)vine_data_deref(msg->io[2].vine_data), "%s",
					 (char*)vine_data_deref(msg->io[1].vine_data) );
			strcat( (char*)vine_data_deref(msg->io[2].vine_data),
					(char*)vine_data_deref(msg->io[0].vine_data) );
			vine_data_mark_ready(vpipe, msg->io[2].vine_data);
		}
		else
			sleep(1);
	}

	vine_talk_exit();
	printf("Consumer shutting down.\n");
	return 0;
}
