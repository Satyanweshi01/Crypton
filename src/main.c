#include "../include/cli.h"
#include "../include/database.h"

int main(void)
{
    if (!open_database())
        return 1;

    if (!create_table())
    {
        close_database();
        return 1;
    }

    if (!close_database())
        return 1;

    start_cli();
    return 0;
}
