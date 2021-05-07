#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <csignal>
#include <fstream>
#include <string>
#include <ext/stdio_filebuf.h>
#include <future>

class UnnamedPipe {
    private:
        int fd[2];
    public:
        UnnamedPipe()
        {
            if(pipe(fd))
            {
                throw std::runtime_error("Failed to create pipe");
            }
        }

        const inline int readFd() const { return fd[0]; }
        const inline int writeFd() const { return fd[1]; }

        void close()
        {
            ::close(fd[0]);
            ::close(fd[1]);
        }

        ~UnnamedPipe() { close(); }
};

class Spawn {
    private:
        UnnamedPipe writePipe;
        UnnamedPipe readPipe;
        UnnamedPipe errorPipe;
    public:
        int child_pid = -1;
        std::unique_ptr<__gnu_cxx::stdio_filebuf<char>> writeBuf = nullptr;
        std::unique_ptr<__gnu_cxx::stdio_filebuf<char>> readBuf = nullptr;
        std::unique_ptr<__gnu_cxx::stdio_filebuf<char>> errorBuf = nullptr;

        std::ostream stdin;
        std::istream stdout;
        std::istream stderr;

        Spawn(const char * path, char * const argv[]): stdin(nullptr), stdout(nullptr), stderr(nullptr)
        {
            child_pid = fork();

            if(child_pid == -1) throw std::runtime_error("Failed to fork a child");
            
            if(child_pid == 0) // child process
            {
                dup2(writePipe.readFd(), STDIN_FILENO);
                dup2(readPipe.writeFd(), STDOUT_FILENO);
                dup2(errorPipe.writeFd(), STDERR_FILENO);

                writePipe.close();
                readPipe.close();
                errorPipe.close();

                if(path == nullptr) exit(1);
                int execResult = execl(path, argv[0], NULL);

                if(execResult == -1)
                {
                    std::cout << "Error: child failed to execute program" << std::endl;
                    exit(1);
                }                

            }
            else //parent process
            {
                close(writePipe.readFd());
                close(readPipe.writeFd());
                
                writeBuf = std::make_unique<__gnu_cxx::stdio_filebuf<char>>(writePipe.writeFd(), std::ios::out);
                readBuf = std::make_unique<__gnu_cxx::stdio_filebuf<char>>(readPipe.readFd(), std::ios::in);
                errorBuf = std::make_unique<__gnu_cxx::stdio_filebuf<char>>(errorPipe.readFd(), std::ios::in);

                stdin.rdbuf(writeBuf.get());
                stdout.rdbuf(readBuf.get());
                stderr.rdbuf(errorBuf.get());
            }
        }

        /* REQ5 */
        void feedWithDataFromFile(std::string inPath)
        {
            std::ifstream cmdStream {inPath};
            if(!cmdStream)
            {
                std::cout << "Cannot open file for reading!" << std::endl;
            }
            else
            {
                std::string line;
                while(std::getline(cmdStream, line))
                {
                    if(line.size())
                    {
                        stdin << line << std::endl;
                    }
                    
                }
            }  
        }

        void send_eof() { writeBuf->close(); }

        int wait() {
            int status;
            waitpid(child_pid, &status, 0);
            return status;
        }
};

using std::string;
using std::cout;
using std::cin;
using std::endl;

Spawn *child_handle = nullptr;
static bool running  = true;
void signalHandler(int signal)
{
    if(child_handle != nullptr)
    {
        child_handle->stdin << "exit" << std::endl;
    }
    std::cout << std::endl << "Press enter to exit." << std::endl;
    running = false;
}

int main(int argc, char** argv)
{
    std::signal(SIGINT, signalHandler);
    char * const argvCh[] = {(char*)"PGen_WORKER"};

    std::string path = "/usr/local/bin/PGen_WORKER";
    char * worker_path = std::getenv("PGEN_WORKER_PATH");
    if(worker_path != nullptr)
    {
        std::cout << "Found variable with PGen_WORKER path: " << worker_path << std::endl;
        path = worker_path;
    }
    else
    {
        std::cout << "Use default path: " << path << std::endl;
    }
    /* REQ3 */
    Spawn child(path.c_str(), argvCh);
    child_handle = &child;

    std::ofstream errFile("log.txt", std::ios::out);
    
    /* REQ8 */
    /* forward error messages to file which stores them */
    auto futureErr = std::async (std::launch::async, [&child, &errFile]()
        {
            string s;
            while(true)
            {
                std::getline(child.stderr, s);
                errFile << s << endl;;
                if(child.stderr.eof())
                {
                    break;
                }
            }    
        });

    /* forward messages from child to user */
    auto futureOut = std::async (std::launch::async, [&child]()
        {
            string s;
            while(true)
            {
                char c = child.stdout.get();                
                // std::getline(child.stdout, s);
                // cout << s << endl;;
                if(child.stdout.eof())
                {
                    break;
                }
                std::cout.put(c);
                std::cout.flush();

            }    
        });

    /* REQ5 */
    if(argc > 1){
        child.feedWithDataFromFile(std::string(argv[1]));
    }    

    string s;
    while(running == true)
    {        
        getline(std::cin, s);
        /* REQ6 */
        child.stdin << s << endl;
    }
    return 0;
}
