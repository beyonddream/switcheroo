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
#include <sys/time.h>
#include <unistd.h>
#include <libproc.h>

#include "include/procer.h"

process_infos_s *procer_process_info_all_init(void)
{
    process_infos_s *process_infos = malloc(sizeof(*process_infos));

    if (process_infos == NULL)
    {
        printf("procer_process_info_all_init::malloc failed.\n");
        exit(1);
    }

    SLIST_INIT(&process_infos->proc_info);

    return process_infos;
}

process_info_s *procer_process_info_init(void)
{
    process_info_s *process_info = malloc(sizeof(*process_info));

    if (process_info == NULL)
    {
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

    if (process_info_view == NULL)
    {
        printf("procer_process_info_view_init::malloc failed.\n");
        exit(1);
    }

    return process_info_view;
}

void procer_process_info_all_deinit(process_infos_s *process_infos)
{

    if (process_infos == NULL)
    {
        return;
    }

    process_info_s *process_info;
    while (!SLIST_EMPTY(&process_infos->proc_info))
    {
        process_info = SLIST_FIRST(&process_infos->proc_info);
        SLIST_REMOVE_HEAD(&process_infos->proc_info, next);
        procer_process_info_deinit(process_info);
    }

    free(process_infos);
    return;
}

void procer_process_info_deinit(process_info_s *process_info)
{

    if (process_info == NULL)
    {
        return;
    }

    if (process_info->name != NULL)
    {
        free(process_info->name);
        process_info->name = NULL;
    }

    process_info_view_s *process_inf_view;
    while (!SLIST_EMPTY(&process_info->proc_info_view))
    {
        process_inf_view = SLIST_FIRST(&process_info->proc_info_view);
        SLIST_REMOVE_HEAD(&process_info->proc_info_view, next);
        procer_process_info_view_deinit(process_inf_view);
    }

    free(process_info);
    return;
}

void procer_process_info_view_deinit(process_info_view_s *process_inf_view)
{

    if (process_inf_view == NULL)
    {
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
    pid_t buf[1];

    kq = kqueue();

    if (kq == -1) {
        printf("Failed to create kqueue.\n");
        return;
    } else {
        printf("kq is creation succeeded.\n");
    }

    int pids[MAX_EVENTS];
    int count = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));

    struct kevent event;
    
    /* Timeout of 10 seconds */
    struct timespec timeout;
    timeout.tv_sec = 10;
    timeout.tv_nsec = 0;

    /* Register the process creation and destruction events */
    for (int i = 0; i < count; i++) {
        EV_SET(&event, pids[i], EVFILT_PROC | EVFILT_SIGNAL, EV_ADD | EV_ENABLE,
        NOTE_EXIT | NOTE_EXEC | NOTE_EXITSTATUS | NOTE_SIGNAL, 0, NULL);
        if (kevent(kq, &event, 1, NULL, 0, &timeout) == -1) {
            perror("Failed to register for process events.\n");
            continue;
        }
        printf("Process events registration succeeded.\n");  
    }

    for(;;) {

        int event_count = kevent(kq, NULL, 0, &event, 1, &timeout);
        if (event_count == -1) {
            printf("Error in kevent. event_count == -1\n");
            goto exit;
        } else {
            printf("event_count is %d\n", event_count);
        }

        for (int i = 0; i < event_count; i++)
        {
            event = events[i];
            pid_t pid = event.ident;
            buf[0] = pid;
            if (event.fflags & NOTE_EXIT) {
                write(1, "e:", 2);
                write(1, buf, 1);
                write(1, "\n", 1);
                printf("Process with PID (%d) has ended.\n", pid);
            } else if (event.fflags & NOTE_EXEC) {
                write(1, "s:", 2);
                write(1, buf, 1);
                write(1, "\n", 1);
                printf("Process with PID (%d) has started.\n", pid);
            } else if (event.fflags & NOTE_EXITSTATUS) {
                write(1, "ex:", 3);
                write(1, buf, 1);
                write(1, "\n", 1);
                printf("Process with PID (%d) has ended.\n", pid);
            } else if (event.fflags & NOTE_SIGNAL) {
                write(1, "s:", 2);
                write(1, buf, 1);
                write(1, "\n", 1);
                printf("Process with PID (%d) has received signal (%ld).\n", pid, event.data);
            } else {
                write(1, "u:", 2);
                write(1, buf, 1);
                write(1, "\n", 1);
                printf("Process with PID (%d) has unknown event.\n", pid);
            }
        }

        sleep(1);
    }

exit:
    close(kq);
    return;
}