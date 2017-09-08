
CURRENTDIR := app

vpath %.c $(CURRENTDIR)/src
vpath %.h $(CURRENTDIR)/inc

CCINCLUDE += -I $(CURRENTDIR)/inc

SRC_FILES := ${notdir ${wildcard $(CURRENTDIR)/src/*.c}}

OBJS_APP := ${patsubst %.c,%.o,$(SRC_FILES)}

OBJS_APP_NAME := ${foreach n,$(OBJS_APP),$(BDIR)/$(n)}





