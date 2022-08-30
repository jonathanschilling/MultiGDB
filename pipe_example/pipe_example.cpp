// https://stackoverflow.com/a/479103

#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#include <string>
#include <iostream>

using namespace std;

#define ASSERT_IS(a,b) if((a)!=(b))exit(-1)
#define ASSERT_NOT(a,b) if((a)==(b))exit(-1)
#define FAIL(msg) fprintf(stderr,"%s\n",msg);exit(-1)

enum PIPE_FILE_DESCRIPTERS
{
  READ_FD  = 0,
  WRITE_FD = 1
};

enum CONSTANTS
{
  BUFFER_SIZE = 100
};

int
main()
{
  int       parentToChild[2];
  int       childToParent[2];
  pid_t     pid;
  string    dataReadFromChild;
  char      buffer[BUFFER_SIZE + 1];
  ssize_t   readResult;
  int       status;

  ASSERT_IS(0, pipe(parentToChild));
  ASSERT_IS(0, pipe(childToParent));

  switch (pid = fork())
  {
    case -1:
      FAIL("Fork failed");
      exit(-1);

    case 0: /* Child */
      ASSERT_NOT(-1, dup2(parentToChild[READ_FD], STDIN_FILENO));
      ASSERT_NOT(-1, dup2(childToParent[WRITE_FD], STDOUT_FILENO));
      ASSERT_NOT(-1, dup2(childToParent[WRITE_FD], STDERR_FILENO));
      ASSERT_IS(0, close(parentToChild [WRITE_FD]));
      ASSERT_IS(0, close(childToParent [READ_FD]));

      /*     file, arg0, arg1,  arg2 */
      execlp("ls", "ls", "-al", "--color", (char *)NULL);

      FAIL("This line should never be reached!!!");
      exit(-1);

    default: /* Parent */
      cout << "Child " << pid << " process running..." << endl;

      ASSERT_IS(0, close(parentToChild [READ_FD]));
      ASSERT_IS(0, close(childToParent [WRITE_FD]));

      while (true)
      {
        switch (readResult = read(childToParent[READ_FD],
                                  buffer, BUFFER_SIZE))
        {
          case 0: /* End-of-File, or non-blocking read. */
            cout << "End of file reached..."         << endl
                 << "Data received was ("
                 << dataReadFromChild.size() << "): " << endl
                 << dataReadFromChild                << endl;

            ASSERT_IS(pid, waitpid(pid, & status, 0));

            cout << endl
                 << "Child exit staus is:  " << WEXITSTATUS(status) << endl
                 << endl;

            exit(0);


          case -1:
            if ((errno == EINTR) || (errno == EAGAIN))
            {
              errno = 0;
              break;
            }
            else
            {
              FAIL("read() failed");
              exit(-1);
            }

          default:
            dataReadFromChild . append(buffer, readResult);
            break;
        }
      } /* while (true) */
  } /* switch (pid = fork())*/
}

