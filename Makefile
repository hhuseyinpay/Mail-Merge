mm:
	gcc -o mm main.c parser.c mail_merge.c dbg.h -I /usr/include/libxml2 -lxml2 -std=c11 -DNDEBUG 
run:
	./mm -t template.xml -v variable.xml -o ../Testcase/test1
