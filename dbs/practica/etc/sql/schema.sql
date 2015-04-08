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
