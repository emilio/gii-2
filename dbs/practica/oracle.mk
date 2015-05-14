all: target/app

include $(ORACLE_HOME)/precomp/lib/env_precomp.mk
PREPRO = bash oracle-prepro
CC = gcc
PREPROCESSOR_FLAGS = -g -DSLXMX_ENABLE -DLTS_ENABLE -D_REENTRANT -I$(ORACLE_HOME)/precomp/public -m64
PREPROCESSOR_LINK_FLAGS = $(PROLDLIBS)
include common.mk
