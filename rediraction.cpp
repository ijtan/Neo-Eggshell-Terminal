#include <iostream>


#include "rediraction.h"

using namespace std;

string truncOut(string name) {
    return NULL;
    FILE *out = freopen(name.c_str(),"w",stdout);
}

string append(string name) {
    return NULL;
    FILE *Appnder = freopen(name.c_str(),"w",stdout);
}

string input(string name) {
    FILE *Appnder = freopen(name.c_str(),"r",stdin);
    return NULL;

}

void pipeTester(){
    int fd[2];
    if (pipe(fd) == -1){
        perror("pipe");
        return;
    }
    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
    }else if (pid == 0){
        close(fd[0]);
        char mesg[] = "HELLO MAN I AM COMINGH";
        write(fd[1],mesg,sizeof(mesg));
        exit(0);
    }else{
        close(fd[1]);
        char bufRead[255];
        int ByteCount = read(fd[0],bufRead, sizeof(bufRead)-1);
        bufRead[ByteCount] = '\0';
        cout<<"recieved: "<<bufRead<<endl;
    }
}
