#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include <f710.h>


int main(int argc, char **argv)
{
        struct f710 c;
        const char *path;
        int ret;

        /* Connect to gamepad. */
        path = (argc >= 2) ? argv[1] : "/dev/input/js0";
        ret = f710_open(&c, path);
        if (ret == -1)
                err(EXIT_FAILURE, "f710_open()");

        /* Print all updates. */
        while (f710_update(&c) != -1)
                f710_print(&c);

        return EXIT_FAILURE;
}
