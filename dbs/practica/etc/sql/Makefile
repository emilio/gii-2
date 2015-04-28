DB := exams

all: target target/global.sql
	sudo -u postgres dropdb $(DB)
	sudo -u postgres psql -c "CREATE DATABASE $(DB);"
	sudo -u postgres psql -f target/global.sql $(DB)

target:
	@mkdir -p $@

target/global.sql: src/schema.sql src/seeds.sql target/triggers.sql
	cat $^ > $@

target/triggers.sql: src/derived_and_triggers.sql target/auto_triggers.sql
	cat $^ > $@

target/auto_triggers.sql: scripts/generate_triggers.sh
	bash $< > $@

.PHONY: all
