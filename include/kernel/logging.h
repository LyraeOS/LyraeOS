#ifndef LYRAEOS_LOGGING_H
#define LYRAEOS_LOGGING_H

#define LOG_INFO(fmt, ...) \
    (kprintf("[{o}INFO{r}] ({s}:{d}) " fmt "\n", 0x60a5fa, __FILE_NAME__, __LINE__, ##__VA_ARGS__))

#define LOG_ERROR(fmt, ...) \
    (kprintf("[{o}ERROR{r}] ({s}:{d}) " fmt "\n", 0xff0000, __FILE_NAME__, __LINE__, ##__VA_ARGS__))

#define LOG_WARNING(fmt, ...) \
    (kprintf("[{o}WARNING{r}] ({s}:{d}) " fmt "\n", 0xffff00, __FILE_NAME__, __LINE__, ##__VA_ARGS__))

#ifdef DEBUG
    #define LOG_DEBUG(fmt, ...) \
        (kprintf("[{o}DEBUG{r}] ({s}:{d}) " fmt "\n", 0xff00ff, __FILE_NAME__, __LINE__, ##__VA_ARGS__))
#else
    #define LOG_DEBUG(fmt, ...) ((void)0)
#endif

#define ASSERT(cond) do { \
        if (!(cond)) panic("\n    ASSERTION FAILED ({s}:{d}) => '" #cond "'", __FILE_NAME__, __LINE__); \
    } while (0)

#endif // LYRAEOS_LOGGING_H
