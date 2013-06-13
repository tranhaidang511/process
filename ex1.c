/*
 ex1.c
 学籍番号 10_15297
 氏名     TRAN DANG HAI
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void child()
{
    printf("PID = %d, PPID = %d\n", getpid(), getppid());
    sleep(1);
    printf("Process %d terminated.\n", getpid());
    exit(0);
}

int main(void)
{
    printf("PID = %d, PPID = %d\n", getpid(), getppid());
    pid_t pid = fork();
    if (pid == -1){
      perror("fork");
      exit(1);
    }
    /* 子プロセスを生成し，次のchild()を子プロセスで実行する
       ように変更しなさい */
    if (pid == 0){
      child();
    }
    else{
      sleep(2);
    }
    /* 生成した子プロセスの終了を待つようにしなさい */
    printf("Process %d terminated.\n", getpid());
    return 0;
}

