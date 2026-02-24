#include <u.h>
#include <libc.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    int afd, lfd, dfd, n;
    char adir[40], ldir[40];
    char buf[256];

    afd = announce("udp!*!11222", adir);
    if(afd < 0)
        return -1;
    for(;;){
        printf("listen\n");
        lfd = listen(adir, ldir);
        if(lfd < 0)
            return -1;
        printf("fork\n");
        switch(fork()){
        case -1:
            perror("forking");
            close(lfd);
            break;
        case 0:
            printf("accept\n");
            dfd = accept(lfd, ldir);
            if(dfd < 0)
                return -1;
            printf("read\n");
            while((n = read(dfd, buf, sizeof(buf))) > 0){
                buf[n] = '\0';
                printf("%s\n", buf);
            }
            exits(nil);
        default:
            close(lfd);
            break;
        }
    }
}
