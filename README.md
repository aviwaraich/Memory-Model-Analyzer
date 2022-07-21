# Memory-Model-Analyzer

Memory-Model-Analyzer will not only display the value of the variables, different from what the so-called dynamical analysis tools would do, but also will analyze with the list the name of the variables in the corresponding memory regions, together with the function to where it belongs, its type and size in the machine where the analysis is being performed.

This Memory-Model-Analyzer program will check the syntax of the code, and its logic, also identify where the variables will be placed in the memory model for processes.

The Memory-Model-Analyzer programn will display the "cumulative" visualization of the variables when considering all the functions and variables definitions within them.

The Memory-Model-Analyzer program will receive one command line argument to specify the file containing the source code to analyze.

Basic Example:

"""
// evil global variables
int evil_glob_var_1;
float evil_glob_var_2;

void fun1(int x)
{
  int y;
  int z;
  printf("%d \n", x+y+z); 
}

int fun2(float z)
{
   float x;
   return (int)(z+x);
}

int main(int argc, char** argv)
{
  int w;

  fun1(w);
  fun2();

  return 0;
}
"""

Result:

"""
>>> Memory Model Layout <<<

***  exec // text ***
   prog1.c

### ROData ###       scope type  size

### static data ###
   evil_glob_var_1   global  int   4
   evil_glob_var_2   global  float   8

### heap ###

####################
### unused space ###
####################

### stack ###
   x   fun1   int   4
   y   fun1   int   4
   z   fun1   int   4
   z   fun2   float   8
   x   fun2   float   8
   argc  main   int   4
   argv main   char**   4
   w  main   int   4

**** STATS ****
  - Total number of lines in the file: 26
  - Total number of functions: 3
    fun1, fun2, main
  - Total number of lines per functions:
    fun1: 3
    fun2: 2
    main: 6
  - Total number of variables per function:
    fun1: 3
    fun2: 2
    main: 3
//////////////////////////////
"""
