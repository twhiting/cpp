//  Created by twhiting on 6/7/18.
//
//  Variadic Templates
//
//      Basic use of a variadic
//      template to print/decorate
//      arguments of a function or
//      macro

#include <iostream>

// -------------------------------------------------

// The 'base' function will be called when the
// last argument was unpacked from Args
template<typename T>
void nlPrint(int line, T t)
{
    std::cout << line << ":\t" << t << std::endl;
}

// Use recursion to incrememnt and print the line number and the latest unpacked variable.
// When 1 argument is left, the 'base-case' function (above) will be called
template<typename T, class... Args>
void nlPrint(int line, T t, Args... args)
{
    std::cout << line << ":\t" << t << std::endl;
    return nlPrint(line + 1, args...);
}

// Variadic Macro
#define NL_DECORATOR(...) nlPrint(1, __VA_ARGS__)

// The 'entry' point into the variadic function.
// Performs identically to the 'NL_DECORATOR(...)' macro
template<class... Args>
void nlPrint(Args... args)
{
    return nlPrint(1, args...);
}

int main(int argc, const char * argv[])
{
    nlPrint("Something clever", 0.11, 8, "Last line");
    
    /*
         OUTPUT:
     
         1:    Something clever
         2:    0.11
         3:    8
         4:    Last line
    
     */
    
    NL_DECORATOR("something cleverer", 0.12, 9, "Other last line");
    
    /*
         OUTPUT:
     
         1:    something cleverer
         2:    0.12
         3:    9
         4:    Other last line
     */
}
