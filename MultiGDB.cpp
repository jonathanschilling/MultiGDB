#include "MultiGDB.h"
#include "ui_MultiGDB.h"

MultiGDB::MultiGDB(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MultiGDB)
{
    ui->setupUi(this);

    connect(ui->btnDoIt, SIGNAL(clicked(bool)), this, SLOT(doItPushed(bool)));
}

MultiGDB::~MultiGDB()
{
    delete ui;
}


void doIt();

void MultiGDB::doItPushed(const bool &) {
    std::cout << "'do it' was pushed" << std::endl;

    doIt();
}






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

void doIt() {
    // IO redirection example from https://stackoverflow.com/a/479103

    int parentToChild[2];
    int childToParent[2];
    pid_t     pid;
    std::string    dataReadFromChild;
    char      buffer[BUFFER_SIZE + 1];
    ssize_t   readResult;
    int       status;

    ASSERT_IS(0, pipe(parentToChild));
    ASSERT_IS(0, pipe(childToParent));

    switch (pid = fork()) {
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
        execlp("gdb", "gdb", "--interpreter=mi", "-ex", "run", "/data/jonathan/work/code/eclipse_wksp/VMEC/Debug/vmec", (char *)NULL);
//        execl("/data/jonathan/work/code/eclipse_wksp/VMEC/Debug/vmec", (char *)NULL);

        FAIL("This line should never be reached!!!");
        exit(-1);
    default: /* Parent */
        std::cout << "Child " << pid << " process running..." << std::endl;

        ASSERT_IS(0, close(parentToChild [READ_FD]));
        ASSERT_IS(0, close(childToParent [WRITE_FD]));

        while (true)
        {
            switch (readResult = read(childToParent[READ_FD], buffer, BUFFER_SIZE)) {
            case 0: /* End-of-File, or non-blocking read. */
                std::cout << "End of file reached..."         << std::endl
                     << "Data received was ("
                     << dataReadFromChild.size() << "): " << std::endl
                     << dataReadFromChild                << std::endl;

                ASSERT_IS(pid, waitpid(pid, &status, 0));

                std::cout << std::endl
                     << "Child exit stauts is:  " << WEXITSTATUS(status) << std::endl
                     << std::endl;

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
                dataReadFromChild.append(buffer, readResult);
                break;

            }
        }
    }
}
