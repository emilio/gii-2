
DROP SEQUENCE grades_id_sequence;
CREATE SEQUENCE grades_id_sequence;
DROP TRIGGER grades_insert;
CREATE TRIGGER grades_insert BEFORE INSERT ON grades
FOR EACH ROW
BEGIN
    SELECT grades_id_sequence.NEXTVAL INTO :new.id FROM dual
END;
/

DROP SEQUENCE teachers_id_sequence;
CREATE SEQUENCE teachers_id_sequence;
DROP TRIGGER teachers_insert;
CREATE TRIGGER teachers_insert BEFORE INSERT ON teachers
FOR EACH ROW
BEGIN
    SELECT teachers_id_sequence.NEXTVAL INTO :new.id FROM dual
END;
/

DROP SEQUENCE subjects_id_sequence;
CREATE SEQUENCE subjects_id_sequence;
DROP TRIGGER subjects_insert;
CREATE TRIGGER subjects_insert BEFORE INSERT ON subjects
FOR EACH ROW
BEGIN
    SELECT subjects_id_sequence.NEXTVAL INTO :new.id FROM dual
END;
/

DROP SEQUENCE themes_id_sequence;
CREATE SEQUENCE themes_id_sequence;
DROP TRIGGER themes_insert;
CREATE TRIGGER themes_insert BEFORE INSERT ON themes
FOR EACH ROW
BEGIN
    SELECT themes_id_sequence.NEXTVAL INTO :new.id FROM dual
END;
/

DROP SEQUENCE asks_id_sequence;
CREATE SEQUENCE asks_id_sequence;
DROP TRIGGER asks_insert;
CREATE TRIGGER asks_insert BEFORE INSERT ON asks
FOR EACH ROW
BEGIN
    SELECT asks_id_sequence.NEXTVAL INTO :new.id FROM dual
END;
/

DROP SEQUENCE answers_id_sequence;
CREATE SEQUENCE answers_id_sequence;
DROP TRIGGER answers_insert;
CREATE TRIGGER answers_insert BEFORE INSERT ON answers
FOR EACH ROW
BEGIN
    SELECT answers_id_sequence.NEXTVAL INTO :new.id FROM dual
END;
/

DROP SEQUENCE exams_id_sequence;
CREATE SEQUENCE exams_id_sequence;
DROP TRIGGER exams_insert;
CREATE TRIGGER exams_insert BEFORE INSERT ON exams
FOR EACH ROW
BEGIN
    SELECT exams_id_sequence.NEXTVAL INTO :new.id FROM dual
END;
/

