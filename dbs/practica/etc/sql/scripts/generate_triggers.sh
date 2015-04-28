#!/bin/bash
for table in grades teachers subjects themes asks answers exams; do
	seq_id="${table}_id_sequence"
    # First we drop the trigger, since if not the sequence refuses to be deleted
	echo "DROP TRIGGER ${table}_insert;"
	echo "DROP SEQUENCE $seq_id;"
	echo "CREATE SEQUENCE $seq_id;"
	echo "CREATE TRIGGER ${table}_insert BEFORE INSERT ON $table"
	echo "FOR EACH ROW"
	echo "BEGIN"
	echo "    SELECT $seq_id.NEXTVAL INTO :new.id FROM dual"
	echo "END;"
	echo "/"
	echo ""
done
