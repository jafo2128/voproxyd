#include "config.h"
#include "daemonize.h"
#include "discovery.h"
#include "errors.h"
#include "log.h"
#include "worker.h"
#include "soap_global.h"
#include "soap_utils.h"

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int g_daemonize = 0;

static void usage(const char *progname)
{
    assert(progname != NULL);
    printf("Usage: %s [-h,--help] [-d,--daemonize]\n", progname);
}

static void parse_daemonize()
{
    if (!optarg) {
        g_daemonize = 1;
        return;
    }

    if (strcmp(optarg, "1") == 0) {
        g_daemonize = 1;
    } else if (strcmp(optarg, "0") == 0) {
        g_daemonize = 0;
    } else {
        fprintf(stderr, "Bad daemonization argument \"%s\". Accepted values are \"0\" or \"1\"\n",
                optarg);
        exit(ERR_INVALID_ARGS);
    }
}

static void parse_arguments(int argc, char *argv[])
{
    struct option const long_options[] = {
        { "daemonize",  optional_argument, NULL, 'd' },
        { "help",       no_argument,       NULL, 'h' },
        { 0,            0,                 0,    0   }
    };
    int opt = 0, option_index = 0;

    while ((opt = getopt_long(argc, argv, "d::h", long_options, &option_index)) != -1) {
        switch (opt) {
        case 'd':
            parse_daemonize();
            break;
        case 'h':
            usage(argv[0]);
            exit(EXIT_SUCCESS);
            break;
        default:
            usage(argv[0]);
            exit(ERR_INVALID_ARGS);
        }
    }

    if (optind < argc) {
        fprintf(stderr, "Invalid option(s): ");

        while (optind < argc) {
            fprintf(stderr, "%s ", argv[optind++]);
        }

        fprintf(stderr, "\n");
        usage(argv[0]);
        exit(ERR_INVALID_ARGS);
    }
}

int main(int argc, char *argv[])
{
    parse_arguments(argc, argv);

    if (g_daemonize) {
        openlog("voproxyd", LOG_NDELAY | LOG_PID, LOG_USER);

        daemonize();
    }

    config_read();

    discovery_init();

    soap_global_construct();

    discovery_do(3000);

    soap_utils_get_services(g_soap, g_config.service_endpoint, &g_services);
    soap_utils_get_profiles(g_soap, soap_utils_get_media_xaddr(&g_services), &g_profiles);
    soap_utils_print_device_info(g_soap, g_config.service_endpoint);
    soap_utils_list_profiles(&g_profiles);

    start_worker();

    soap_global_destruct();
    config_destruct();
    discovery_destruct();
}

