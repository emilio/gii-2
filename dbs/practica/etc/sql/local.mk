DB := exams

all: target target/global.sql
	sudo -u postgres dropdb $(DB)
	sudo -u postgres psql -c "CREATE DATABASE $(DB);"
	sudo -u postgres psql -f target/global.sql $(DB)

include common.mk
