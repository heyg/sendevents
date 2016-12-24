#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/input.h>

#define VERSION "0.1.0"

#define S_ALL ( S_IRUSR \
              | S_IWUSR \
              | S_IXUSR \
              | S_IRGRP \
              | S_IWGRP \
              | S_IXGRP \
              | S_IROTH \
              | S_IWOTH \
              | S_IXOTH )

#define BUF_SIZ 32

unsigned long long readUsec(char *token) {
    unsigned long long work;
    unsigned long long usec;
    char *cp;

    token[strlen(token) - 1] = '\0';
    token += 1;
    work = strtoull(token, &cp, 10);
    usec = work * 1000000;  // from milli sec to nano sec
    work = strtoull(cp + 1, NULL, 10);
    usec += work;
    return usec;
}

int main(int argc, char *argv[]) {
    struct input_event ev;
    int fd = 0;
    FILE *fp = NULL;
    int rc;
    char buf[BUF_SIZ];
    unsigned long long pusec = 0;
    unsigned long long cusec = 0;

    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        printf("sendevents version: %s\n", VERSION);
        return EXIT_SUCCESS;
    }

    if (argc < 3) {
        fprintf(stderr, "usage: sendevents <device-file> <input-file>\n");
        fprintf(stderr, "       sendevents -v\n");
        return EXIT_FAILURE;
    }

    fd = open(argv[1], O_CREAT | O_WRONLY, S_ALL);
    if (fd <= 0) {
        fprintf(stderr, "could not open %s, %s\n", argv[2], strerror(errno));
        return EXIT_FAILURE;
    }

    fp = fopen(argv[2], "r");
    if (fp == NULL) {
        fprintf(stderr, "can't open input file:%s\n", argv[1]);
        return EXIT_FAILURE;
    }

    while (fscanf(fp, "%s %hx %hx %x", buf, &ev.type, &ev.code, &ev.value) != EOF) {
        cusec = readUsec(buf);
        if (pusec != 0 && pusec != cusec) {
            usleep((unsigned int) (cusec - pusec));
        }
        rc = write(fd, &ev, sizeof(ev));
        if (rc < sizeof(ev)) {
            fprintf(stderr, "write event failed, %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        memset(&ev, 0, sizeof(ev));
        pusec = cusec;
    }

    fclose(fp);
    close(fd);
    return EXIT_SUCCESS;
}

