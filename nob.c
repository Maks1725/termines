#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "gcc", "main.c");
    nob_cmd_append(&cmd, "-o", "termines");
    nob_cmd_append(&cmd, "-Wall", "-Wextra");
    nob_cmd_append(&cmd, "-lncurses");

    if (!nob_cmd_run(&cmd)) {
        return 1;
    }

    return 0;
}
