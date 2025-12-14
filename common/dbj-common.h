#ifndef DBJ_STRING_INCLUDED
#define DBJ_STRING_INCLUDED

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
static inline void *dbj_memccpy(void * /*restrict*/ dst, const void * /*restrict*/ src, int c, size_t n)
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
#endif

/*
   dbj capi string is an char array in a struct, for example

   struct { char data[0xFF]; } str1 = {""};

   if size is runtime value the resulting instance can not be initialized

   int j = 0xFF ;
   struct { char data[j]; } str1 = {""}; // does not compile
   struct { char data[j]; } str1 ; // does compile

   this is static thus free-ing is not required
   this can be sent into and out of functions and dbj components,
   C runtime will do very fast copy elision in any case

   obviously wchar_t variant can and will be added as soon as it is required
   same is for utf-8, utf-16 and utf-32

   Godbolt quick test: https://godbolt.org/z/4sjT7bY7n
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

#undef DBJ_COUNT_OF
#define DBJ_COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#undef DBJ_STRING_TYPE
#define DBJ_STRING_TYPE(SIZE_) \
    struct dbj_string_##SIZE_  \
    {                          \
        char data[SIZE_];      \
    }

// type names defined like that can clash and will clash
// thus we will define few probably most common
typedef DBJ_STRING_TYPE(1024) dbj_string_1024;
typedef DBJ_STRING_TYPE(512) dbj_string_512;
typedef DBJ_STRING_TYPE(256) dbj_string_256;
typedef DBJ_STRING_TYPE(128) dbj_string_128;
typedef DBJ_STRING_TYPE(64) dbj_string_64;
typedef DBJ_STRING_TYPE(32) dbj_string_32;
typedef DBJ_STRING_TYPE(16) dbj_string_16;
typedef DBJ_STRING_TYPE(8) dbj_string_8;

/*
this is dbj string variable declaration anonymous struct used thus no name clashes

static const DBJ_STRING( str_global, 123 ) = {{"static const global dbj string"}};

    printf("%s", str_global.data );

    there is no need for a size function, inside is just an array with known size:

    printf("%zu", sizeof str_global.data );

*/
#undef DBJ_STRING
#define DBJ_STRING(NAME_, SIZE_) \
    struct                       \
    {                            \
        char data[SIZE_];        \
    } NAME_ // = {.size = SIZE_} <-- no can do for VLA, if SIZE_ is a runtime value!

#undef DBJ_STRING_SIZE

// Q: why don't we use sizeof( STR_.data ) bellow?
// A: because that is data size not string size
//    string size is defined with '\0' (EOS) it will and can be
//    smaller than sizeof data , ditto size of data
//    is (available) capacity.
#define DBJ_STRING_SIZE(STR_) DBJ_COUNT_OF(STR_.data)

#undef DBJ_STRING_CAPACITY
#define DBJ_STRING_CAPACITY(STR_) sizeof(STR_.data)

#undef DBJ_STRING_ASSIGN

// consider this macro name as the guidance
// this does not work if target or is not zero terminated
// S_ is string literal
#define DBJ_STRING_ASSIGN(STR_, S_)                             \
    do                                                          \
    {                                                           \
        dbj_memccp(STR_.data, S_, '\0', sizeof(STR_.data) - 1); \
    } while (0)

//  is important
// Any field can be in 3 states: exist, does not exist and exist but **empty**
// In databases field is always there but can be empty too
// we define as empty an dbj string the one that has '\0' as the first char
//

#undef DBJ_STRING_SET_EMPTY
#define DBJ_STRING_SET_EMPTY(STR_) (STR_.data[0] = '\0')

#undef DBJ_STRING_IS_EMPTY
#define DBJ_STRING_IS_EMPTY(STR_) (STR_.data[0] == '\0')

DBJ_EXTERN_C_END

#endif // DBJ_STRING_INCLUDED