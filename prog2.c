/***************************************************************************//**

  @file         main.c


  @author       Stephen Brennan;
                Revised by Addison Fenrick, Flynn Peterson, Ben McKee

  @date

  @brief

*******************************************************************************/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
  Function Declarations for builtin shell commands:
 */

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_about(char ** args);
/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    2222 Add code here:
     Add at least one builtin command
    
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "about"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit,
  &lsh_about
};

int lsh_num_builtins() {
    //printf("%d",sizeof(builtin_str));
    return 4;//sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}



//about function
int lsh_about(char ** args)
{
  printf("Group Memebers- Addison, Ben, & Flynn\n");
  printf("For CS3230- Operating Systems\n");

return 1;
}


/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int lsh_help(char **args)
{
  int i;
  printf("Stephen Brennan's LSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int lsh_exit(char **args)
{
  return 0;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
  pid_t pid;
  int status;
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
        perror("lsh");
            }
            exit(EXIT_FAILURE);
          } else if (pid < 0) {
            // Error forking
            perror("lsh");
          } else {
            // Parent process
            do {
              waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
          }
          return 1;
        }

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */
 
//444444 Add code here:
 
int myPipe(char **args) {

    pid_t pid;
    int status;
    int pipefd[2]; // file descriptors for the pipe

    // create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork(); // create a new process

    if (pid == -1) { // check for error
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { // child process
        // close the input end of the pipe
        close(pipefd[1]);
        // map stdout to the output end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        // execute child code using execvp() function
        
        int temp = -1;
        int i;
        for (i = 0; i < lsh_num_builtins(); i++) {
         //   printf("cmp: %d\n",strcmp(args[0], builtin_str[i]));
          if (strcmp(args[0], builtin_str[i]) == 0) {
              
              temp = i;
             }
        }
      //  printf("%d\n",temp);
        if(temp!=-1)
        {
            (*builtin_func[temp])(args);
        }
        else{
            lsh_launch(args);
      //      perror("execvp error");
        }
       // printf("\n\n");
       // perror("execvp"); // print error message if failed
        exit(EXIT_FAILURE);
    }
    else { // parent process
        // close the output end of the pipe
        close(pipefd[1]);
        // map stdin to the input end of the pipe
        dup2(pipefd[1], STDIN_FILENO);
        // execute parent code using execvp() function
        int temp = -1;
        int i;
        for (i = 0; i < lsh_num_builtins(); i++) {
         //   printf("cmp: %d\n",strcmp(args[0], builtin_str[i]));
          if (strcmp(args[2], builtin_str[i]) == 0) {
              
              temp = i;
             }
        }
      //  printf("%d\n",temp);
        if(temp!=-1)
        {
            (*builtin_func[temp])(args+2);
        }
        else{
            lsh_launch(args+2);
      //      perror("execvp error");
        }
        printf("\n\n");
      //  perror("execvp"); // print error message if failed
    
    }
    wait(&status);
    return 1;
}

/*
create a new process:
parent process: wait()
         Child process:
(1) create a pipe()
                (2) create a new process fork()
                    Parent process: (a) close pipe output end (b) use dup2() to map stdin (c) use exec() family function to load parent code
                    Child process: (a) close pipe input end (b)use dup2() to map stdout (c) use exec() family function to load child code
}
 
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    55555 Add code here:
 
     int myAmphersand(char **args)
    {
     create a new process:
     parent process: (1) create a new process and child uses exec() family function to load child code(2)wait()
         Child process: use exec() family function to load child code
 
    }
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
int myAmphersand(char **args)
{
    //printf(*args);
    pid_t pid;
       int status;
    int e=0;
    int amp =-1;
    while (args[e]!=0)
    {
        if (strcmp(args[e],"&")==0){
            amp = e;
        }
        e++;
    }
       pid = fork(); // create a new process

       if (pid == -1) {
           perror("fork error");
           exit(1);
       }

       if (pid == 0) { // child process
          char** firstargs = malloc(sizeof(char*)*amp);
           memcpy(firstargs, args, sizeof(char*)*amp);
           //char *args[] = {"ls", "-l", NULL}; // first command
        //   printf("pid==0\n");
           //printf("%d\n",lsh_num_builtins());
           int temp = -1;
           int i;
           for (i = 0; i < lsh_num_builtins(); i++) {
            //   printf("cmp: %d\n",strcmp(args[0], builtin_str[i]));
             if (strcmp(firstargs[0], builtin_str[i]) == 0) {
                 
                 temp = i;
                }
           }
         //  printf("%d\n",temp);
           if(temp!=-1)
           {
               (*builtin_func[temp])(firstargs);
           }
           else{
               lsh_launch(firstargs);
         //      perror("execvp error");
           }
           
           exit(1);
       } else { // parent process
          // wait(&status); // wait for child to finish
           //char *args[] = {"echo", "Child process finished", NULL}; // second command
           int temp = -1;
           int i;
           for (i = 0; i < lsh_num_builtins(); i++) {
            //   printf("cmp: %d\n",strcmp(args[0], builtin_str[i]));
             if (strcmp(args[amp+1], builtin_str[i]) == 0) {
                 
                 temp = i;
                }
           }
         //  printf("%d\n",temp);
           if(temp!=-1)
           {
               (*builtin_func[temp])(args[amp+1]);
           }
           else{
               lsh_launch(args[amp+1]);
              // perror("execvp error");
           }
           printf("\n\n");
         //
       }
    wait(&status);
    return 1;
}
/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    66666 Add code here:
  int mySemicolon(char **args)
    {
     create a new process:
     parent process:  (1)wait() (2) create a new process for the second child (3) wait()
         Child process: (1) use exec() family function to load child code
 
    }
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

int mySemicolon(char ** args)
{
   // for (int i =0; i < args.len; i= i+2)
   // {
        pid_t pid1, pid2;
        
        // Create the first child process
        pid1 = fork();
        
        if (pid1 == -1) {
            // If fork() returns -1, an error occurred
            perror("Error creating first child process");
            exit(EXIT_FAILURE);
        } else if (pid1 == 0) {
            // This is the first child process
           // printf(args[0]);
           // printf("\n\n");
            int temp = -1;
            int i;
            for (i = 0; i < lsh_num_builtins(); i++) {
             //   printf("cmp: %d\n",strcmp(args[0], builtin_str[i]));
              if (strcmp(args[0], builtin_str[i]) == 0) {
                  
                  temp = i;
                 }
            }
          //  printf("%d\n",temp);
            if(temp!=-1)
            {
                (*builtin_func[temp])(args);
            }
            else{
                lsh_launch(args);
          //      perror("execvp error");
            }
            printf("\n\n");
           // perror("Error executing child1");
            exit(EXIT_FAILURE);
        } else {
            // This is the parent process
            // Wait for the first child process to finish
            waitpid(pid1, NULL, 0);//
            // Create the second child process
            pid2 = fork();
            
            if (pid2 == -1) {
                // If fork() returns -1, an error occurred
               // perror("Error creating second child process");
                exit(EXIT_FAILURE);
            } else if (pid2 == 0) {
                // This is the second child process
                
                
                int temp = -1;
                int i;
                for (i = 0; i < lsh_num_builtins(); i++) {
                 //   printf("cmp: %d\n",strcmp(args[0], builtin_str[i]));
                  if (strcmp(args[2], builtin_str[i]) == 0) {
                      
                      temp = i;
                     }
                }
              //  printf("%d\n",temp);
                if(temp!=-1)
                {
                    (*builtin_func[temp])(args+2);
                }
                else{
                    lsh_launch(args+2);
              //      perror("execvp error");
                }
              
                printf("\n");
               //int a = (*builtin_func[])(args)
                //execvp(args[2],builtin_func);
               // perror("Error executing child2");
                exit(EXIT_FAILURE);
            } else {
                // This is the parent process
                // Wait for the second child process to finish
                waitpid(pid2, NULL, 0);
            }
        }
   // }
        return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
  int i;
  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }
  
    for(int j = 0; args[j] != NULL; j++)
    {
        
        if(strcmp(args[j], "|") == 0)
        {
           // printf("Pipe\n");
            return myPipe(args);
        }
       else if(strcmp(args[j], "&") == 0)
        {
            
            return myAmphersand(args);;
        }
        else if(strcmp(args[j], ";") == 0)
        {
           // printf("Semicolon\n");
            return mySemicolon(args);
        }
    }
    
    for (i = 0; i < lsh_num_builtins(); i++) {
      if (strcmp(args[0], builtin_str[i]) == 0) {
        return (*builtin_func[i])(args);
      }
    }
    

  return lsh_launch(args);
}

#define LSH_RL_BUFSIZE 1024
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *lsh_read_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        free(tokens_backup);
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void lsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    //printf("> ");

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    1111  add code here:e
        Print your own promo, such as cs3230-Emily>>
    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
    printf("cs3230-Addison, Ben, & Flynn>>");
    line = lsh_read_line();
    args = lsh_split_line(line);

    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}





