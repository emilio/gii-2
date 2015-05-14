#!/bin/bash
for table in grades teachers subjects themes questions answers exams; do
	seq_id="${table}_id_sequence"
	echo "DROP SEQUENCE $seq_id;"
	echo "CREATE SEQUENCE $seq_id START WITH 1 INCREMENT BY 1;"
	echo "CREATE TRIGGER ${table}_insert BEFORE INSERT ON $table"
	echo "FOR EACH ROW"
	echo "BEGIN"
	echo "    SELECT $seq_id.NEXTVAL INTO :new.id FROM dual;"
	echo "END;"
	echo "/"
	echo ""
done
