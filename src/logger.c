#include "../include/logger.h"
#include "../include/model.h"
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <sys/stat.h>
#endif

#define LOGGER_PATH_SIZE 1024
#define MAX_PREFIX_SIZE 32

static void get_logger_path(char *path, size_t path_size)
{
#ifdef _WIN32
    char exe_path[LOGGER_PATH_SIZE];
    char *last_slash;

    if (GetModuleFileNameA(NULL, exe_path, sizeof(exe_path)) == 0)
    {
        snprintf(path, path_size, ".\\log\\logs.txt");
        return;
    }

    last_slash = strrchr(exe_path, '\\');
    if (!last_slash)
    {
        snprintf(path, path_size, ".\\log\\logs.txt");
        return;
    }

    *last_slash = '\0';
    snprintf(path, path_size, "%s\\log", exe_path);
    _mkdir(path);
    snprintf(path, path_size, "%s\\log\\logs.txt", exe_path);
#else
    mkdir("./log", 0755);
    snprintf(path, path_size, "./log/logs.txt");
#endif
}

static date current_date_time(void)
{
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    date result = {0};

    if (!local)
        return result;

    result.year = local->tm_year + 1900;
    result.month = local->tm_mon + 1;
    result.day = local->tm_mday;
    result.hour = local->tm_hour;
    result.minute = local->tm_min;
    result.second = local->tm_sec;

    return result;
}

// logs information
void log_msg(char *msg)
{
    char loggerPath[LOGGER_PATH_SIZE];
    char prefix[MAX_PREFIX_SIZE];
    date timestamp = current_date_time();
    FILE *fp;

    get_logger_path(loggerPath, sizeof(loggerPath));

    fp = fopen(loggerPath, "a");

    if (fp == NULL)
        return;

    snprintf(prefix, sizeof(prefix), "%02d-%02d-%04d %02d:%02d:%02d  ", timestamp.day, timestamp.month, timestamp.year, timestamp.hour, timestamp.minute, timestamp.second);

    fprintf(fp, "\n");
    fprintf(fp, prefix);
    fprintf(fp, msg);

    fclose(fp);
}
