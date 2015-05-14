# Can't use := because of encina's make, and can't use
# ifeq(...)...
CC = clang
PREPRO = ecpg
PREPROCESSOR_LINK_FLAGS = -lecpg
PREPROCESSOR_FLAGS = -I/usr/include/postgresql -std=c99 -DPOSTGRES
include common.mk
