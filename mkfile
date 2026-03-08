xpl: xpl.c
    6c -c xpl.c
    6l -o xpl xpl.6

clean:
	rm -rf *.6 xpl
