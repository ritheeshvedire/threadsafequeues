#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>

/**
 * Default program options
 */
struct program_options {
    int num_producers = 1;     
    int num_consumers = 1;     
    int num_items = 10;        
    int sleep_time_ms = 10;    
    size_t queue_capacity = 100; //capacity for bounded
    std::string queue_type = "unbounded"; // unbounded or bounded
    bool thread_safe_cout = false; // thread safe cout to streamline output when multiple producers and consumers are present
};

void print_help(const char* program_name);
void parse_command_line_arguments(int argc, char* argv[], program_options& options);

#endif // OPTIONS_H