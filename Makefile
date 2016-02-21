CC=gcc-5.3.0
CFLAGS+=-Wall -Wextra -m32 -g
LDFLAGS+=-m32 -g
EXECUTABLEFILE=simulator

sources= main.c Parser.c SysCfg.c

all:$(EXECUTABLEFILE)
.PHONY:all

%.d:%.c
	set -e;\
	rm -f $@;\
	$(CC) -MM $(CFLAGS) $< > $@.;\
	sed 's,/($*/)/.o[ :]*,/1.o $@ : ,g' < $@.> $@;\
	rm -f $@.

include $(sources:.c=.d)
objs = $(sources:.c=.o)

$(EXECUTABLEFILE):$(objs)
	$(CC) $(LDFLAGS) $(objs) -o $(EXECUTABLEFILE)
	@echo 执行$(EXECUTABLEFILE)
	@date "+%Y-%m-%d %k:%M:%S"
	@echo
	@echo
	@./$(EXECUTABLEFILE)
	@echo
	@echo
	@echo 执行结束
	@date "+%Y-%m-%d %k:%M:%S"

clean:
	-rm -rf *.o $(EXECUTABLEFILE) *.d. *.d
