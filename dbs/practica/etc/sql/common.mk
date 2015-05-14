target:
	@mkdir -p $@

target/global.sql: src/schema.sql target/triggers.sql src/view.sql src/seeds.sql
	cat $^ > $@

target/triggers.sql: src/derived_and_triggers.sql target/auto_triggers.sql
	cat $^ > $@

target/auto_triggers.sql: scripts/generate_triggers.sh
	bash $< > $@

.PHONY: all
