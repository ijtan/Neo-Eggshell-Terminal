#Welcome to Egg Shell!
##Introduction
In this project I will be attempting to create a mini shell, from which I can execute external commands,
execute internal commands, interact with environment Variables (create/retrieve).

## Execution

To execute simply Run neoEggshell from the /build/ folder

```
./neoEggshell
```

## Internal Commands

## TODO: split into features / additions (Reidrects piping etc)

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
