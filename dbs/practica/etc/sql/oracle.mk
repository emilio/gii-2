all: target target/global.sql
	sqlplus / @src/oracle-drops.sql
	sqlplus / @target/global.sql
include common.mk
