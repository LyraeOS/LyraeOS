#ifndef LYRAEOS_LOGGING_H
#define LYRAEOS_LOGGING_H
#include <drivers/display/tty.h>

#define LOG_INFO(fmt, ...) \
    (kprintf("[{info}INFO{reset}] ({s}:{d}) " fmt "\n", __FILE_NAME__, __LINE__, ##__VA_ARGS__))

#define LOG_ERROR(fmt, ...) \
    (kprintf("[{error}ERROR{reset}] ({s}:{d}) " fmt "\n", __FILE_NAME__, __LINE__, ##__VA_ARGS__))

#define LOG_WARNING(fmt, ...) \
    (kprintf("[{warning}WARNING{reset}] ({s}:{d}) " fmt "\n", __FILE_NAME__, __LINE__, ##__VA_ARGS__))

#ifdef DEBUG
    #define LOG_DEBUG(fmt, ...) \
        (kprintf("[{accent}DEBUG{reset}] ({s}:{d}) " fmt "\n", __FILE_NAME__, __LINE__, ##__VA_ARGS__))
#else
    #define LOG_DEBUG(fmt, ...) ((void)0)
#endif

#define ASSERT(cond) do { \
        if (!(cond)) panic("\n    ASSERTION FAILED ({s}:{d}) => '" #cond "'", __FILE_NAME__, __LINE__); \
    } while (0)

#endif // LYRAEOS_LOGGING_H
