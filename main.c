#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>


static int poll_gpio(const char *gpio_name)
{
    printf("gpio_name=[%s]\n", gpio_name);

    // get gpio file descriptor
    int fd = open(gpio_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    // set poll structure with filedescriptor and needed events
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLPRI; // | POLLERR;

    // loop wait for events on gpio
    size_t sread = 0;
    char buffer[256] = {0};
    while (1) {

        if (poll(&pfd, 1, -1) == -1)
            perror("poll");

        // check if event happened
        if (pfd.revents & POLLPRI) {

            sread = read(fd, buffer, sizeof(buffer));
            if (sread == -1) {
                perror("read");
                return -1;
            } else {
                printf("buffer[%s][%ld]\n", buffer, sread);
                memset(buffer, 0, sizeof(buffer));
            }

        }
    }

    // close gpio file descriptor
    close(fd);

    return 0;
}


int main(int argc, char *argv[])
{
    if (argc == 2)
        poll_gpio(argv[1]);
    else
        printf("Usage:%s <gpio_pin>\n", argv[0]);

    return 0;
}

