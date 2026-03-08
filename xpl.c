#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <plumb.h>

static int port = 11222;

int
main(int argc, char *argv[])
{
    Plumbmsg plumb;
    Plumbattr attr;
    int pfd, afd, lfd, dfd, n;
    char addr[16], adir[40], ldir[40];
    char buf[128];
    char range[16];
    char *s;

    plumb.src = "xpl";
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
    snprint(addr, 16, "udp!*!%d", port);
    afd = announce(addr, adir);
    if(afd < 0){
        perror("announce");
        return -1;
    }
    for(;;){
        lfd = listen(adir, ldir);
        if(lfd < 0)
            return -1;
        switch(fork()){
        case -1:
            perror("fork");
            close(lfd);
            break;
        case 0:
            dfd = accept(lfd, ldir);
            if(dfd < 0){
                perror("accept");
                return -1;
            }
            memset(buf, 0, sizeof(buf));
            memset(range, 0, sizeof(range));
            if((n = read(dfd, buf, sizeof(buf))) > 0){
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
