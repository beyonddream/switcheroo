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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>

#include "include/procer.h"

process_infos_s *procer_process_info_all_init(void)
{
    process_infos_s *process_infos = malloc(sizeof(*process_infos));

    if (process_infos == NULL) {
        printf("procer_process_info_all_init::malloc failed.\n");
        exit(1);
    }

    SLIST_INIT(&process_infos->proc_info);

    return process_infos;
}

process_info_s *procer_process_info_init(void)
{
    process_info_s *process_info = malloc(sizeof(*process_info));

    if (process_info == NULL) {
        printf("procer_process_info_init::malloc failed.\n");
        exit(1);
    }

    process_info->pid = -1;
    process_info->name = NULL;
    SLIST_INIT(&process_info->proc_info_view);

    return process_info;
}

process_info_view_s *procer_process_info_view_init(void)
{
    process_info_view_s *process_info_view = malloc(sizeof(*process_info_view));

    if (process_info_view == NULL) {
         printf("procer_process_info_view_init::malloc failed.\n");
         exit(1);
    }

    return process_info_view;
}

void procer_process_info_all_deinit(process_infos_s *process_infos)
{
    
    if (process_infos == NULL) {
        return;
    }

    process_info_s *process_info;
    while(!SLIST_EMPTY(&process_infos->proc_info)) {
        process_info = SLIST_FIRST(&process_infos->proc_info);
        SLIST_REMOVE_HEAD(&process_infos->proc_info, next);
        procer_process_info_deinit(process_info);
    }

    free(process_infos);
    return;
}

void procer_process_info_deinit(process_info_s *process_info)
{

    if (process_info == NULL) {
        return;
    }

    if (process_info->name != NULL) {
        free(process_info->name);
        process_info->name = NULL;
    }

    process_info_view_s *process_inf_view;
    while(!SLIST_EMPTY(&process_info->proc_info_view)) {
        process_inf_view = SLIST_FIRST(&process_info->proc_info_view);
        SLIST_REMOVE_HEAD(&process_info->proc_info_view, next);
        procer_process_info_view_deinit(process_inf_view);
    }

    free(process_info);
    return;
}

void procer_process_info_view_deinit(process_info_view_s *process_inf_view)
{

    if (process_inf_view == NULL) {
        return;
    }

    free(process_inf_view);
    return;
}

const char *procer_get_name(void) 
{
    const char *current_process_name = "what_the_proc";
    
    return current_process_name;
}

void procer_start_process_listener(void)
{
    struct kevent events[MAX_EVENTS];
    int kq;

    kq = kqueue();
    
    if (kq == -1) {
        printf("Failed to create kqueue.\n");
        return;
    }

    /* Register the process creation and destruction events */
    struct kevent event;
    EV_SET(&event, 1, EVFILT_PROC, EV_ADD | EV_ENABLE, NOTE_EXIT | NOTE_EXEC, 0, NULL);
    if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
        printf("Failed to register for process events.\n");
        return;
    }    

    /* Create a file and open them in write/append mode */
    printf("going to create ./procer.dat");
    FILE *file = fopen("./procer.dat", "w");
    if (file == NULL) {
        printf("Failed to open procer.dat\n");
        return;
    }

    for(;;) {
        int event_count = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (event_count == -1) {
            printf("Error in kevent.\n");
            return;
        }

        struct kevent event;
        for (int i = 0; i < event_count; i++) {
            event = events[i];
            pid_t pid = event.ident;
            if (event.fflags & NOTE_EXIT) {
                fputs("Process ended:", file);
                fputs(pid, file);
                fputs("\n", file);
                printf("Process with PID (%d) has ended.\n", pid);
            } else if (event.fflags & NOTE_EXEC) {
                fputs("Process started:", file);
                fputs(pid, file);
                fputs("\n", file);
                printf("Process with PID (%d) has started.\n", pid);
            }
        }
    }

    return;
}   