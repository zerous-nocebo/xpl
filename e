#!/bin/rc

if(! ~ $1 /*){
    echo  `{pwd}/$1 | ncat -u 127.0.0.1 11222
}
if not{
    if(! test -f $1){
        touch $1
    }
    echo $1 | ncat -u 127.0.0.1 11222
}
