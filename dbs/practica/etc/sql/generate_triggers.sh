#!/bin/bash
TARGET="auto_increment_triggers.sql"

echo "" > $TARGET
for table in grades teachers subjects themes asks answers exams; do
	seq_id="${table}_id_sequence";
	echo "DROP SEQUENCE $seq_id;" >> $TARGET;
	echo "CREATE SEQUENCE $seq_id;" >> $TARGET;
	echo "DROP TRIGGER ${table}_insert;" >> $TARGET;
	echo "CREATE TRIGGER ${table}_insert BEFORE INSERT ON $table" >> $TARGET;
	echo "FOR EACH ROW" >> $TARGET;
	echo "BEGIN" >> $TARGET;
	echo "    SELECT $seq_id.NEXTVAL INTO :new.id FROM dual" >> $TARGET;
	echo "END;" >> $TARGET;
	echo "/" >> $TARGET;
	echo "" >> $TARGET;
done
