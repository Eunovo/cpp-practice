#include <iostream>

// Logging function
#define LOG(log_level, fmt, ...) { printf("[%s] %s " fmt "\n", log_level, __FUNCTION__, ##__VA_ARGS__); }

// If parameter is not true, test fails
// This check function would be provided by the test framework
#define IS_TRUE(x) { if (!(x)) LOG("ERROR", "failed on line %i", __LINE__); }
