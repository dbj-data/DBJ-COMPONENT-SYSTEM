#include "dbj-string.h"
#include <stdio.h>

#define SX(F, X) printf("\n%-32s : " F, (#X), (X))
// bool's only
#define SB(X) printf("\n%-32s : %s", (#X), (X ? "true" : "false"))

// this is NOT declared as constant
// listen to your compiler warnings
// and fix them by removing const if you plan to modify
static DBJ_STRING(str_global, 0xFF) = {"declare define and initialize a global dbj string"};

dbj_string_16 driver(dbj_string_32 from)
{
    dbj_string_16 to = {0};
    // will be cut to 16 if larger
    DBJ_STR_REUSE(to, from.data);
    return to;
}

/*
NOTE: After this function,
stack level will be back where it was
Heap is not touched + there is no leak.
*/
static void tester(void)
{
    // single or double brackets is OK
    dbj_string_16 str = driver((dbj_string_32){"Hello World!Hello World?"});
    SX("%s", str.data);
    str = driver((dbj_string_32){"ABC"});
    SX("%s", str.data);
    SB(DBJ_STRING_IS_EMPTY(str));
    SX("%p", DBJ_STRING_SET_EMPTY(str));
    SB(DBJ_STRING_IS_EMPTY(str));
}

int main(void)
{
    SX("DBJ string is:\t %s", str_global.data);
    SX("Size is:\t %zu", DBJ_STRING_SIZE(str_global));
    SX("Capacity is:\t %zu", DBJ_STRING_CAPACITY(str_global));
    // assign new value
    // make sire str_global is not const if you plan to do this
    DBJ_STR_REUSE(str_global, "Hello DBJ String World!");
    SX("DBJ string is:\t %s", str_global.data);
    SX("Size is:\t %zu", DBJ_STRING_SIZE(str_global));
    SX("Capacity is:\t %zu", DBJ_STRING_CAPACITY(str_global));

    tester();
    return 42;
}