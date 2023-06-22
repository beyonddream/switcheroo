/**
* 
* MIT License
* 
* Copyright (c) 2023 Arun a.k.a beyonddream

* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef __PROCER_H__
#define __PROCER_H__

#include <sys/queue.h>
#include <time.h>

#define MAX_EVENTS 32

/* Info about a process from time of foreground to background view change */
typedef struct process_info_view {
    time_t start_time;
    time_t end_time;
    SLIST_ENTRY(process_info_view) next;
} process_info_view_s;

/* Info about a particular process */
typedef struct process_info {
    int pid;
    char *name;
    SLIST_ENTRY(process_info) next;
    SLIST_HEAD(, process_info_view) proc_info_view;
} process_info_s;

/* Info about a collection of processes */
typedef struct process_infos {
    SLIST_HEAD(, process_info) proc_info;
} process_infos_s;

/* Main API routines */
const char *procer_get_name(void);
void procer_start_process_listener(pid_t file);

/* Init routines */
process_infos_s *procer_process_info_all_init(void);
process_info_s *procer_process_info_init(void);
process_info_view_s *procer_process_info_view_init(void);

/* De-Init routines */
void procer_process_info_all_deinit(process_infos_s *);
void procer_process_info_deinit(process_info_s *);
void procer_process_info_view_deinit(process_info_view_s *);

#endif /* __PROCER_H__ */