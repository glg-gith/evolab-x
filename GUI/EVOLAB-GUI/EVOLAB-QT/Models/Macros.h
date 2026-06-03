#ifndef MACROS_H
#define MACROS_H


#include <iostream>
#include <LabConsts.h>
#include "StringUtils.h"


#include <stdio.h>
#include <dlfcn.h>


//extern "C" {

//const char **symbol_access(const char *symbol_name);
//const char *str_n00 = "hkjhkhk";

#define concat(a, b)                    a##b
#define define_assoc0(name, value)      char *concat(const_id_, value)=#name; \
                                        enum { name = value }
//#define define_assoc1(name)              \
//    constexpr char *s_##name = #name; \
//    const uintptr_t uid0 = reinterpret_cast<uintptr_t>(s_##name); \
//    constexpr uintptr_t uid = 12; \
//    enum { name = uid }

//#define define_assoc2(name)              \
//    const std::string *ss_##name = new st::string(#name); \
//    const std::string s_##name = *ss_##name; \
//    constexpr uintptr_t uid_##name = ((uintptr_t)s_##name); \
//    enum { name = uid0 }
////constexpr uintptr_t uid = 12; \

//#define define_assoc(name)              \
//    const uintptr_t s_##name = (const uintptr_t)#name; \
//    enum { name = s_##name }
#define define_assoc(name)      const char *myVar_##name=#name; \
                                enum { name = 1 };


#define STR(x) QUOTEME(x)
#define UNSTR(x)  (##x)
//#define UNSTR2(x) ""+STR(#x)
#define STR_TO_MACRO(m, t) [] () { /*std::string s = ReplaceAll2(m, "\"", " ");*/ return m; } ()


#define VOID                void
#define VOID_               double
#define BOOL                bool
#define INT                 int
//#define CONST_INT           const int
#define DOUBLE              double
//#define CONST_DOUBLE        const double
//---
//#define BOOL_F(x)           bool x = false;
//#define INT_F(x)            int x = 0;
//#define DOUBLE_F(x)         double x = 0.0f;
//#define SET(x, v)           x = v;

#define INT_PTR             int *
#define DOUBLE_PTR          double *
#define FREE_PTR(x)         delete x;
#define RETURN(x)           return x;
#define RETURN_             RETURN(0)
//---
//#define INT_PTR_F(x)        int *x = new int();
//#define DOUBLE_PTR_F(x)     double *x = new double();
//#define SET_PTR(x, v)       *x = v;


// Reserved
#define BEGIN               {
#define END                 }
#define BLANK

// Define new const, var, ptr or func...
//#define CONST_TYPE(t)       const t
//#define CONST_NAME(n)       n
//#define CONST_VAL(v)        = v;
//---
#define DEF_CONST(t, n, v)      const t n = v;
#define DEF_VAR(t, n, v)        t n = v;
#define DEF_PTR(t, n, v)        t *n = new t(); *n = v;
#define DEF_FUNC_(type, name)   type name(...)\
                                BEGIN\
                                ...\
                                END

#define DEF_FUNC(type, name)    type name(...)\nBEGIN\n...\nEND\n
//const char *DEF_FUNC_str_name = (const char *)"DEF_FUNC(type, name)";


// Set var or ptr value...
#define SET_VAR(x, v)       x = v;
#define SET_PTR(x, v)       *x = v;




class Macros
{


//}

//--------------------------------------------------------


public:


    Macros() {
        /*clog << NewConst << endl;*/ clog << NewConst2 << endl;
        define_assoc(DEF_FUNC);
        clog << ">> A: Found SYMBOL _DEF_FUNC: " << myVar_DEF_FUNC << endl;
        clog << ">> B: Found SYMBOL _DEF_FUNC dlsym: " << get_macro_value("DEF_FUNC") << endl;
        clog << ">> C: Found SYMBOL _DEF_FUNC dlsym: " << get_macro_value("myVar_DEF_FUNC") << endl;
        clog << ">> D: Found SYMBOL _DEF_FUNC dlsym: " << get_macro_value("_GLOBAL__sub_I_myVar_DEF_FUNC") << endl;
    }


    static const char **symbol_access(const char *symbol_name)
    {
        clog << "Symbol name 2: " << symbol_name << endl;
        static void * handle;
        if(handle || (handle = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL | RTLD_GLOBAL)))
        {
            // Do some error checking
            char* lError = dlerror();
            if (lError)
            {
                // This error doesn't get hit
                clog << "Error: " << lError << endl;
                return NULL;
            } else {
                clog << "NO ERROR !!!!!!!!!!!! => Return is: " << (const char **)dlsym(handle, symbol_name) << endl;
                return (const char **)dlsym(handle, symbol_name);
            }
        }
        //fprintf(stderr, "Could not access symbol table: %s\n", dlerror());
        clog << "Could not access symbol table: " << dlerror() << endl;
        return NULL;
    }


    static const char *get_macro_value(const char *macro_name)
    {
        clog << "Macro name: " << macro_name << endl;
        char symbol_name[24];
        //sprintf(symbol_name, "_%s", macro_name);
        sprintf(symbol_name, "%s", macro_name);
        clog << "Symbol name: " << symbol_name << endl;
        //if (symbol_access("_GLOBAL__sub_I__DEF_FUNC" /*symbol_name*/))
        if (symbol_access(symbol_name))
            return *(symbol_access(symbol_name));
        else
            return "Sprotch !!";//*(symbol_access(symbol_name)); //symbol_name; //
    }



//    typedef struct {
//        std::string str;
//    } MACRO;

//    CONST_TYPE(DOUBLE)
//    CONST_NAME(NewConst)
//    CONST_VAL(
//            0.000001f
//            )

    //CONST(DOUBLE, NewConst2)
    DEF_CONST(DOUBLE, NewConst2,
              0.000002f
              )

    ////    static void define_macro_assoc();
    //    {
    //        define_assoc(DEF_FUNC);
    //    }


};


#endif // MACROS_H
