/*
 * Copyright 2004-2016 Cray Inc.
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

#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"

#define CHPL_MPI_NUM_RANKS "--mpi-num-ranks"

static char* mpi_num_ranks=NULL;


static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  int numranks;
  
  // Get the number of ranks
  // TODO -- if we want, we can also use an environment variable, although
  // that's probably overkill for this.
  if (mpi_num_ranks==NULL) {
    numranks = numLocales;
  } else {
    numranks = atoi(mpi_num_ranks);
  }

  chpl_compute_real_binary_name(argv[0]);

  sprintf(baseCommand, "mpirun -np %d %s %s", numranks, MPIRUN_XTRA_OPTS, 
          chpl_get_real_binary_name());

  //size = strlen(MPIRUN_PATH) + 1 + strlen(baseCommand) + 1;
  size = strlen(baseCommand)+1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_mem_allocMany(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  
  //sprintf(command, "%s/%s", MPIRUN_PATH, baseCommand);
  sprintf(command, "%s", baseCommand);
  for (i=1; i<argc; i++) {
    strcat(command, " '");
    strcat(command, argv[i]);
    strcat(command, "'");
  }

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  return chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                                  argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {
  // handle --mpi-num-ranks <nrank> or --mpi-num-ranks=<nrank>
  if (!strcmp(argv[argNum], CHPL_MPI_NUM_RANKS)) {
    mpi_num_ranks = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_MPI_NUM_RANKS"=", strlen(CHPL_MPI_NUM_RANKS))) {
    mpi_num_ranks = &(argv[argNum][strlen(CHPL_MPI_NUM_RANKS)+1]);
    return 1;
  }

  return 0;
}


void chpl_launch_print_help(void) {
  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  fprintf(stdout, "  %s : specify number of MPI ranks (default : numLocales)\n", CHPL_MPI_NUM_RANKS);
}
