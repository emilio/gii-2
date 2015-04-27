#!/bin/bash

TARGET=".tmp.sql"
cat schema.sql > $TARGET;
./generate_triggers.sh;
cat auto_increment_triggers.sql >> $TARGET;
cat derived_and_triggers.sql >> $TARGET;
cat seeds.sql >> $TARGET;
./olivo-exec.sh $TARGET $TARGET.result --transaction
rm $TARGET;
