int main() {
  int i;
  char **args; 

  while(1) {
    printf("yongfeng's shell:~$ ");
    args = get_line();
    if (strcmp(args[0], "exit") == 0) exit(0);     /* if it's built-in command exit, exit the shell */
    if('&&') parse_out_two_commands: cmd1, cmd2;
    if (execute(cmd1) != -1)   /* if cmd1 successfully executed */
      execute(cmd2);       /* then execute the second cmd */
  }
}

int execute(char **args){
  int pid;
  int status;   /* location to store the termination status of the terminated process */
  char **cmd;   /* pure command without special charactors */

  if(pid=fork() < 0){   //fork a child process, if pid<0, fork fails
    perror("Error: forking failed");
    return -1;
 }

 /* child */
 else if(pid==0){             /* child process, in which command is going to be executed */
   cmd = parse_out(args);
   /* codes handleing I/O redirection */

   if(execvp(*cmd, cmd) < 0){   /* execute command */
     perror("execution error");
     return -1;
   }
  return 0;
 }

 /* parent */
 else{         /* parent process is going to wait for child or not, depends on whether there's '&' at the end of the command */
  if(strcmp(args[sizeof(args)],'&') == 0){
     /*  handle signals */
  }
   else if (pid = waitpid(pid, &status, 0) == -1) perror("wait error");
  }
}