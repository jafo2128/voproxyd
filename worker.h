#pragma once

extern int g_current_event_fd;

void worker_init();
void worker_start();
void worker_add_udp_fd(int fd);
void worker_do_external_discovery();

