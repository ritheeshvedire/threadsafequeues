#include "options.h"
#include <iostream>
#include <getopt.h>
#include <cstdlib>

void print_help(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS]\n"
              << "Options:\n"
              << "  -P, --producers <N>       Number of producers (default: 1)\n"
              << "  -C, --consumers <N>       Number of consumers (default: 1)\n"
              << "  -N, --num-items <N>       Number of items to produce/consume (default: 10)\n"
              << "  -S, --sleep-time <N>      Sleep time in milliseconds (default: 10)\n"
              << "  -B, --queue-capacity <N>  Queue capacity for bounded queue (default: 100)\n"
              << "  -T, --queue-type <type>   Queue type (unbounded or bounded, default: unbounded)\n"
              << "  -F, --thread-safe-cout    Thread-safe console output. If enabled, cout is thread safe (default:false)\n"
              << "  -h, --help                Show this help message\n";
}

void parse_command_line_arguments(int argc, char* argv[], program_options& options) {
    const char* const short_opts = "P:C:N:S:B:T:f:h";
    const option long_opts[] = {
        {"producers", required_argument, nullptr, 'P'},
        {"consumers", required_argument, nullptr, 'C'},
        {"num-items", required_argument, nullptr, 'N'},
        {"sleep-time", required_argument, nullptr, 'S'},
        {"queue-capacity", required_argument, nullptr, 'B'},
        {"queue-type", required_argument, nullptr, 'T'},
        {"thread-safe-cout", no_argument, nullptr, 'F'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}
    };

    while (true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if (opt == -1) {
            break;
        }

        switch (opt) {
            case 'P':
                options.num_producers = std::stoi(optarg);
                break;
            case 'C':
                options.num_consumers = std::stoi(optarg);
                break;
            case 'N':
                options.num_items = std::stoi(optarg);
                break;
            case 'S':
                options.sleep_time_ms = std::stoi(optarg);
                break;
            case 'B':
                options.queue_capacity = std::stoul(optarg);
                break;
            case 'T':
                options.queue_type = optarg;
                break;
            case 'F':
                options.thread_safe_cout = true;
                break;
            case 'h':
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                print_help(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}