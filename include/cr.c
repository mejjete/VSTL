#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    open(argv[1], O_CREAT, _S_IRWXU);
    return 0;
}