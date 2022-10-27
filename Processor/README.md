# That directory contains the program that emulates cpu called proc 

**That is the most briliant code for that type of problem that human been could ever seen**

The program is created as my student project


# How to build

```terminal
  $ git clone https://github.com/IraIvanov/Project_H    
  $ cd Project_H/Processor    
  $ make        
  ```
After building project you'll receive 2 executable files, compiler and proc.

compiler program is created for compilating code that is written on R_ASM, "unique" version of assembler language that my cpu uses.

Let's look at some syntax rules:

  At first every piece of code starts with command, for example 
    
    IN
    
  That line will put number, that was input by keyboard into vertual stack
  
  Some commands, like : POP, PUSH, JMP (and it's variations), CALL require arguments
  
    JMP :label
   
  That line will move programs ip to the line, where :label is defined 
  
  Also you can put comments into your code 
  
    Push 1 ; pushed 1 into stack
   
  Last part of the line, starting from ';' will be ingored during compilation
  
You can see some exampels, for example program factorial.txt counting factorial of input number

Also you can build your own code:

```terminal
  $ ./compiler example_code.txt        
  ```

After that you'll receive binary file called a.out, to execute it use:

```terminal
  $ ./proc a.out        
  ```

Multiple files :white_check_mark:       
Warnings :negative_squared_cross_mark:      

**Have fun!**     
![Alt-текст](https://derpicdn.net/img/2019/5/6/2032644/large.jpg)
