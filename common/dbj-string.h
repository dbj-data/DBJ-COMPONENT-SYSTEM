#ifndef DBJ_STRING_INCLUDED
#define DBJ_STRING_INCLUDED

#include <stddef.h> // size_t
#include <string.h> // strnlen

// officialy not until C23
#if __STDC_VERSION__ <= 201710L
// _memccpy in MSVC
// https://developers.redhat.com/blog/2019/08/12/efficient-string-copying-and-concatenation-in-c
/*
struct Book {
    int id;
    char name[0xFF];
};

static inline struct Book *book_populate(struct Book *bptr_,
                                         char *const new_name_) {
    assert(bptr_);
    bptr_->id = 13;
    memccpy(bptr_->name, new_name_, '\0', 0xFF);
    return bptr_;
}

that memccpy above is much faster then snprintf(bptr_->name, 0xFF, "%s", new_name_ );
*/
void *dbj_memccpy(void * /*restrict*/ dst, const void * /*restrict*/ src, int c, size_t n);

void *dbj_memccpy(void * /*restrict*/ dst, const void * /*restrict*/ src, int c, size_t n)
{
    const char *s = src;
    for (char *ret = dst; n; ++ret, ++s, --n)
    {
        *ret = *s;
        if ((unsigned char)*ret == (unsigned char)c)
            return ret + 1;
    }
    return 0;
}
#else
#include <string.h>
#define dbj_memccpy memccpy
#endif // __STDC_VERSION__

/*
   DBJ string is a char array wrapped in a struct:

   struct { char data[0xFF]; } str1 = {""};

   Compile-time size (constant) allows initialization:
      struct { char data[0xFF]; } str1 = {""}; // OK - compiles

   Runtime size (VLA) prohibits initialization:
      int j = 0xFF;
      struct { char data[j]; } str1 = {""}; // ERROR - does not compile
      struct { char data[j]; } str1;        // OK - uninitialized

   Stack-based allocation means no malloc/free required.
   Can be passed to/from functions and DBJ components efficiently.
   C runtime performs fast copy elision automatically.

   Wide character variants (wchar_t, UTF-8, UTF-16, UTF-32) will be added when needed.
*/

#undef DBJ_EXTERN_C_BEGIN
#undef DBJ_EXTERN_C_END

#ifdef __cplusplus
#define DBJ_EXTERN_C_BEGIN \
    extern "C"             \
    {
#define DBJ_EXTERN_C_END }
#else // ! __cplusplus
#define DBJ_EXTERN_C_BEGIN
#define DBJ_EXTERN_C_END
#endif // !__cplusplus

DBJ_EXTERN_C_BEGIN

#undef DBJ_STRING_TYPE
#define DBJ_STRING_TYPE(SIZE_) \
    struct dbj_string_##SIZE_  \
    {                          \
        char data[SIZE_];      \
    }

// DBJ String names predefined like this can clash and will clash
// in larger projects with other libraries that define similar constructs
// thus we will define here few probably most common
// this will make users use these as commion names
// and avoid name clashes
typedef DBJ_STRING_TYPE(1024) dbj_string_1024;
typedef DBJ_STRING_TYPE(512) dbj_string_512;
typedef DBJ_STRING_TYPE(256) dbj_string_256;
typedef DBJ_STRING_TYPE(128) dbj_string_128;
typedef DBJ_STRING_TYPE(64) dbj_string_64;
typedef DBJ_STRING_TYPE(32) dbj_string_32;
typedef DBJ_STRING_TYPE(16) dbj_string_16;
typedef DBJ_STRING_TYPE(8) dbj_string_8;

/*

static const DBJ_STRING( str_global, 123 ) = {{"static const global dbj string"}};

    printf("%s", str_global.data );

    there is no need for a capacity function, inside is just an array with known size:

    printf("Capacity: %zu", sizeof str_global.data );

*/
#undef DBJ_STRING
#define DBJ_STRING(NAME_, SIZE_) \
    struct                       \
    {                            \
        char data[SIZE_];        \
    } NAME_ // = {.size = SIZE_} <-- no can do for VLA, if SIZE_ is a runtime value!

#undef DBJ_STRING_SIZE

// returns the actual string length (number of characters before null terminator)
// up to the capacity of the dbj string
// thus safe and fast use of strnlen
#define DBJ_STRING_SIZE(STR_) strnlen(STR_.data, sizeof(STR_.data))

#undef DBJ_STRING_CAPACITY
#define DBJ_STRING_CAPACITY(STR_) sizeof(STR_.data)

#undef DBJ_STR_REUSE

// consider this macro name as the warning
// it reuses the target dbj string instance
// this does not work if target is const or is not zero terminated
// S_ is string literal
// thus consider this for assignment only
#define DBJ_STR_REUSE(STR_, S_)                                  \
    do                                                           \
    {                                                            \
        dbj_memccpy(STR_.data, S_, '\0', sizeof(STR_.data) - 1); \
    } while (0)

// Following is important
// "field" is datababase concept
// Any field can be in 3 states: exist, does not exist and exist but **empty**
// In databases field is "always there" and can be empty or not
// we define as empty an dbj string the one that has '\0' as the first char
//

#undef DBJ_STRING_SET_EMPTY
#define DBJ_STRING_SET_EMPTY(STR_) (STR_.data[0] = '\0')

#undef DBJ_STRING_IS_EMPTY
#define DBJ_STRING_IS_EMPTY(STR_) (STR_.data[0] == '\0')

DBJ_EXTERN_C_END

#endif // DBJ_STRING_INCLUDED