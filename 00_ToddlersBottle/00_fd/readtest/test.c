#include <fcntl.h>   // open
#include <unistd.h>  // read, write, close
#include <stdio.h>   // perror, printf

#define N 128

int main(void)
{
    // int fd1 = 0;
    int fd1 = open("file1.txt", O_RDONLY);
    if (fd1 < 0) { perror("read open"); return 1; }

    int fd2 = open("file2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd2 < 0) { perror("write open"); return 2; }

    printf("fd1 = %d, fd2 = %d\n", fd1, fd2);

    char buf[N];
    while (1) {
        int n = read(fd1, buf, N);
        printf("\n[n = %d]\n", n);
        if (n <= 0) break;
        write(fd2, buf, n);
    }
    close(fd1);
    close(fd2);
}
