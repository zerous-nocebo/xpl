#include <u.h>
#include <libc.h>
#include <plumb.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    Plumbmsg plumb;
    Plumbattr attr;
    int plumbsendfd;
    char *fname;
    
    fname = "mem.c";
    plumb.src = "plumbtest";
    plumb.dst = "edit";
    plumb.wdir = "/usr/glenda/src/echo";
    plumb.type = "text";
    plumb.attr = &attr;
    attr.name = "addr";
    attr.value = "1";
    attr.next = NULL;
    plumb.ndata = strlen(fname);
    plumb.data = fname;
    
    plumbsendfd = plumbopen("send", OWRITE);
    if(plumbsendfd < 0){
        perror("plumbopen");
        return -1;
    }
    printf("%d\n", plumbsendfd);
    if(plumbsend(plumbsendfd, &plumb) < 0){
        perror("plumbsend");
        return -1;
    }
    return 0;
}
