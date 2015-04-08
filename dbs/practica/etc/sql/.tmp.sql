-- Oracle does not support DROP _ IF EXISTS... -.-
DROP TABLE grades CASCADE CONSTRAINTS;
CREATE TABLE grades (
	id NUMBER(10) NOT NULL,
	name VARCHAR(255) NOT NULL,
	PRIMARY KEY(id)
);

DROP TABLE teachers CASCADE CONSTRAINTS;
CREATE TABLE teachers (
	id NUMBER(10) NOT NULL,
	dni VARCHAR(10) NOT NULL UNIQUE,
	name VARCHAR(255) NOT NULL,
	knowledge_field VARCHAR(255) NOT NULL,
	PRIMARY KEY (id)
);

DROP TABLE subjects CASCADE CONSTRAINTS;
CREATE TABLE subjects (
	id NUMBER(10) NOT NULL,
	code VARCHAR(10) NOT NULL UNIQUE,
	name VARCHAR(255) NOT NULL,
	grade_id NUMBER(10) NOT NULL,
	PRIMARY KEY (id),
	FOREIGN KEY (grade_id) REFERENCES grades(id) ON DELETE CASCADE
);

DROP TABLE themes CASCADE CONSTRAINTS;
CREATE TABLE themes (
	id NUMBER(10) NOT NULL,
	-- since "order" is reserved...
	-- Also this way is more flexible
	priority NUMBER(5) DEFAULT 1,
	name VARCHAR(255) NOT NULL,
	description CLOB NOT NULL,
	subject_id NUMBER(10) NOT NULL,
	PRIMARY KEY (id),
	FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE
);

DROP TABLE asks CASCADE CONSTRAINTS;
CREATE TABLE asks (
	id NUMBER(10) NOT NULL,
	statement VARCHAR(255) NOT NULL,
	PRIMARY KEY (id)
);

DROP TABLE answers CASCADE CONSTRAINTS;
CREATE TABLE answers (
	id NUMBER(10) NOT NULL,
    title VARCHAR(255) NOT NULL,
    is_correct CHAR(1) CHECK (is_correct in (0,1)),
    priority NUMBER(5) DEFAULT 1,
    ask_id NUMBER(10) NOT NULL,
	PRIMARY KEY (id),
    FOREIGN KEY (ask_id) REFERENCES asks(id) ON DELETE CASCADE
);

DROP TABLE exams CASCADE CONSTRAINTS;
CREATE TABLE exams (
	id NUMBER(10) NOT NULL,
    year NUMBER(10) NOT NULL,
    convocatory NUMBER(10) NOT NULL,
    subject_id NUMBER(10) NOT NULL,
	PRIMARY KEY (id),
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE,
    CONSTRAINT unique_exam UNIQUE (subject_id, year, convocatory)
);

DROP TABLE asks_themes CASCADE CONSTRAINTS;
CREATE TABLE asks_themes (
	ask_id NUMBER(10) NOT NULL,
	theme_id NUMBER(10) NOT NULL,
	FOREIGN KEY (ask_id) REFERENCES asks(id) ON DELETE CASCADE,
	FOREIGN KEY (theme_id) REFERENCES themes(id) ON DELETE CASCADE,
    PRIMARY KEY (ask_id, theme_id)
);

DROP TABLE asks_exams CASCADE CONSTRAINTS;
CREATE TABLE asks_exams (
	ask_id NUMBER(10) NOT NULL,
	exam_id NUMBER(10) NOT NULL,
	FOREIGN KEY (ask_id) REFERENCES asks(id) ON DELETE CASCADE,
	FOREIGN KEY (exam_id) REFERENCES exams(id) ON DELETE CASCADE,
    PRIMARY KEY (ask_id, exam_id)
);

DROP TABLE subjects_teachers CASCADE CONSTRAINTS;
CREATE TABLE subjects_teachers (
	subject_id NUMBER(10) NOT NULL,
	teacher_id NUMBER(10) NOT NULL,
	FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE,
	FOREIGN KEY (teacher_id) REFERENCES teachers(id) ON DELETE CASCADE,
    PRIMARY KEY (subject_id, teacher_id)
);

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

ALTER TABLE asks ADD exam_count NUMBER(10) NOT NULL;
CREATE TRIGGER ask_exam_insert AFTER INSERT ON asks_exams
FOR EACH ROW
BEGIN
	UPDATE asks SET exam_count = exam_count + 1 WHERE id = :new.ask_id;
END;
/

CREATE TRIGGER ask_exam_delete AFTER DELETE ON asks_exams
FOR EACH ROW
BEGIN
	UPDATE asks SET exam_count = exam_count - 1 WHERE id = :old.ask_id
END;
/
