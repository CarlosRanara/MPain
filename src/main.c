#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers.h"

int main()
{
    int exit_flag;
    databaseType db;

    exit_flag = 0;

    initData(&db);
    loadData(&db);

    while (!exit_flag) {
        mainMenu(&db, &exit_flag);
    }

    saveData(&db);

    return 0;
}