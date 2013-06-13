/*
 ex2.c
 学籍番号 10_15297
 氏名     TRAN DANG HAI
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 50
#define MAX_LEN  500

int main(void)
{
  int argc, argc1, n = 0, comc, fd[2], i;
  char input[MAX_LEN], *comm[MAX_LEN], *argv[MAX_ARGS], *argv1[MAX_ARGS], *cp, hist[100][MAX_ARGS], num[3];
    const char *delim = " \t\n"; /* コマンドのデリミタ(区切り文字) */

    while (1) {
        /* プロンプトの表示 */
        ++n;
        printf("command[%d] ", n);

        /* 1行読み込む */
        if (fgets(input, sizeof(input), stdin) == NULL) {
	  /* EOF(Ctrl-D)が入力された */
	  printf("Goodbye!\n");
	  exit(0);
        }
	/*ヒストリ機能*/
	if (!strcmp(input,"!!\n")){
	  strcpy(input, hist[n - 1]);
	}
	if ((input[0] == '!') && (input[1] != '!')){
	  i = 0;
	  while (input[i] != '\n'){
	    num[i] = input[i + 1];
	    i++;
	  }
	  num[i] = '\0';
	  strcpy(input, hist[atoi(num)]);
	}
	strcpy(hist[n], input);
	if (!strcmp(hist[n],"history\n")){
	  for (i = 1; i < n; i++){
	    printf("  %d  %s", i, hist[i]);
	  }
	  continue;
	}
        /* コマンド行を空白/タブで分割し，配列 argv[] に格納する */
	if (strchr(input,'|') == NULL){ //簡単なコマンド
	  cp = input;
	  for (argc = 0; argc < MAX_ARGS; argc++) {
            if ((argv[argc] = strtok(cp,delim)) == NULL)
	      break;
            cp = NULL;
	  }
	  if (!strcmp(argv[0],"exit")){
            printf("Goodbye!\n");
            exit(0);
	  }
	  int status;
	  pid_t pid = fork();
	  if (pid == -1){
            perror("fork");
            exit(1);
	  }       
	  if (pid == 0){
            execvp(argv[0],argv);//子プロセスを作って、execによる実装する
            exit(0);
	  }
	  else{
	    wait(&status);
	    continue;
	  }
	}
	else{ //パイプ機能
	  cp = input;
	  for (comc = 0; comc < 2; comc++) {
            if ((comm[comc] = strtok(cp,"|")) == NULL)
	      break;
            cp = NULL;
	  }
	  cp = comm[0];
	  for (argc = 0; argc < MAX_ARGS; argc++) {
            if ((argv[argc] = strtok(cp,delim)) == NULL)
	      break;
            cp = NULL;
	  }
	  cp = comm[1];
	  for (argc1 = 0; argc1 < MAX_ARGS; argc1++) {
            if ((argv1[argc1] = strtok(cp,delim)) == NULL)
	      break;
            cp = NULL;
	  }
	  pipe(fd);
	  int status;
	  pid_t pid = fork();
	  if (pid == -1){
            perror("fork");
            exit(1);
	  }       
	  if (pid == 0){
	    dup2(fd[1],1);
            execvp(argv[0],argv);
	    close(fd[1]);
            exit(0);
	  }
	  else{
	    wait(&status);
	    close(fd[1]);
	    int status1;
	    pid_t pid1 = fork();
	    if (pid1 == -1){
	      perror("fork");
	      exit(1);
	    }       
	    if (pid1 == 0){
	      dup2(fd[0],0);
	      close(fd[0]);
	      execvp(argv1[0],argv1);
	      exit(0);
	    }
	    else{
	      wait(&status1);
	      close(fd[0]);
	    }
	    continue;
	  }
	}
        /*
          1. 入力したコマンドを実行できるようにしなさい.

          2. exitコマンドで終了できるようにしなさい.

          3. パイプ機能を追加しなさい．

          4. その他ユーザが使いやすくなるようにシェルの機能を強化しなさい.
             例えば：
             - cd コマンド
             - ヒストリ機能
             - コマンドの最後に & を付けたときにバックグラウンド実行する
        */
    }
}
