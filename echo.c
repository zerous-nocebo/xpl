#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <plumb.h>

int
main(int argc, char *argv[])
{
    Plumbmsg plumb;
    Plumbattr attr;
    int pfd, afd, lfd, dfd, n;
    char adir[40], ldir[40];
    char buf[128];
    char range[16];
    char *s;

    plumb.src = "echo";
    plumb.dst = "edit";
    plumb.wdir = "/mnt/term";
    plumb.type = "text";
    plumb.attr = &attr;
    attr.name = "addr";
    attr.value = "0";
    attr.next = NULL;
    
    pfd = plumbopen("send", OWRITE);
    if(pfd < 0){
        perror("plumbopen");
        return -1;
    }
    afd = announce("udp!*!11222", adir);
    if(afd < 0)
        return -1;
    for(;;){
//        printf("listen\n");
        lfd = listen(adir, ldir);
        if(lfd < 0)
            return -1;
//        printf("fork\n");
        switch(fork()){
        case -1:
            perror("forking");
            close(lfd);
            break;
        case 0:
//            printf("accept\n");
            dfd = accept(lfd, ldir);
            if(dfd < 0)
                return -1;
//            printf("read\n");
            
            memset(buf, 0, sizeof(buf));
            memset(range, 0, sizeof(range));
            if((n = read(dfd, buf, sizeof(buf))) > 0){
                //printf("read %ld bytes\n", n);
                buf[n] = 0;
                if((s = strchr(buf, '\n')) != nil)
                    *s = 0;
                if((s = strchr(buf, ':')) != nil){
                	strcpy(range, s+1);
                	*s = 0;
                }
                s = buf;
                if(buf[0] == '/')
                    s++;
                
                print("n: %ld: %s\n", n, s);
                plumb.ndata = strlen(s);
                plumb.data = s;
                attr.value = range;
                if(plumbsend(pfd, &plumb) < 0){
                    perror("plumbsend");
                    return -1;
                }
            }
            exits(nil);
        default:
            close(lfd);
            break;
        }
    }
}
