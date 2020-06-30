# Welcome to Egg Shell!
## Introduction
In this project I will be attempting to create a mini shell, from which I can execute external commands,
execute internal commands, interact with environment Variables (create/retrieve).

## Compilation
This project uses cmake which greatly faciliattes the compilation process. 
CMake version 3.16+\
To Compile Simply run:
```
cmake CMakeLists.txt
```


## Execution
C++ Version 20\
To execute simply Run 'neoEggshell' from the build folder

```
./neoEggshell
```

## Internal Commands

```
- echo
- unset <variable name>
- MYVAR=MYVALUE                     (for setting environment variables).
- showenv
- listproc                          (Show suspended processes)
- <command 1> | <command 2> | ...   (Piping)
- <command> * <filneame>            (Redirects where * = ['>'/'>>'/'<'])
- source <filename>                 (Read and parse commands line by line from text file)
- bg
- <command> &                       (Ampersnad after commands -> do not wait for them to finish)
- cd <dir>
- exit
```
## Internal Commands
External commands can be run normally by entering them in the prompt. 
## Features
multiple commands can be entered in one line by using semicolons:
```
command1;command 2
```
White Space can be escaped by using Quotes
```
mkdir "Hello World"
```

If the entry does not contain an even number of quotes, the user is asked for more input:
```
mkdir "hel
llo"
```

## Issues
When using the ampersand '&' feature, the output does not come as expcted, and comes all over the terminal (Probably an issue with the prompt and the outputs happening simulatenously). The same prpblem occurs when using the bg command, as two outputs happen simultaneously. 

The bg and '&' features also leave behind zombie processes as they are never waited for. 




