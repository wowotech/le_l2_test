#
# Name:    Makefile
#
# Purpose: makefile of this project
#
# Copyright (C) 2017 wowotech
# Subject to the GNU Public License, version 2.

# Created By:         wowo<www.wowotech.net>
# Created Date:       2017-01-20
# 
# ChangeList:
# Created in 2017-01-20 by wowo;
#

#CC      = /opt/arm-2012.03/bin/arm-none-linux-gnueabi-gcc
#CC      = /opt/gcc-linaro-aarch64-linux-gnu-4.8-2013.12_linux/bin/aarch64-linux-gnu-gcc
CC      = gcc

CFLAGS  = -Wall -O3 -static
INCFLAGS=
LDFLAGS = -static -lm -lbluetooth

TARGETS = lel2_server lel2_client

.PHONY:all 
all:$(TARGETS)

lel2_server:$(OBJS) lel2_server.o
	$(CC) -o $@ $^ $(LDFLAGS)

lel2_client:$(OBJS) lel2_client.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCFLAGS)

%.d:%.c
	@set -e; rm -f $@; $(CC) -MM $< $(INCFLAGS) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY:clean 
clean:
	rm -f $(TARGETS) *.exe *.o *.d *.d.*
