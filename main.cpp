#include <iostream>
#include <stdio.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <signal.h>

void killme(int) {
/*
    printf("Killing self\n");
    fflush(stdout);
*/
    execl("killall -2 script.sh", "");
    killpg(getpgrp(), SIGKILL);
    raise(SIGINT);
}

int main() {
    const auto pid = fork();
    if (pid == -1 ) {
        std::cout << "Error" << std::endl;
        return 1;
    } else if (pid == 0) {
        // Create process group with "this" process as a leader
        setpgid(pid, 0);
        if (signal(SIGINT, killme) == SIG_ERR) {
            std::cout << "Cannot set signal handler" << std::endl;
            exit(1);
        }
        std::cout << "I am child" << std::endl;
        // With SIGKILL, the first child dies immediately after the parent ends.
        // However, SIGKILL cannot be trapped in the script and so the trap approach cannot be used.
        if (auto ret = prctl(PR_SET_PDEATHSIG, SIGINT); ret == -1) {
            std::cout << "prctl ret=" << ret << std::endl;
            perror(0);
            exit(1);
        }
        // The suspicion was that the first child is /bin/bash...
        //execl("/bin/bash", "sh", "-c", "../script.sh", (char *)0);
        // ... so the script is being launched immediately
        execl("../script.sh", "");
        // However, 'pstree -s `pidof processes`' displays
        // systemd───lightdm───lightdm───x-session-manag───yakuake───bash───mc───bash───sudo───processes───script.sh───sleep
        // in both cases. Also, running the script directly should be the same as using /bin/bash -c as the original script
        // contains she-bang at the beginning which launches bash as well.
        std::cout << "Terminating child" << std::endl;
        return 0;
    } else {
        std::cout << "I am parent" << std::endl;
        sleep(5);
    }
    std::cout << "Terminating parent" << std::endl;
    return 0;
}