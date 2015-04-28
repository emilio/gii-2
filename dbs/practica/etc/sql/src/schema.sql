CREATE TABLE grades (
	id INTEGER NOT NULL,
	name VARCHAR(255) NOT NULL,
	PRIMARY KEY(id)
);


CREATE TABLE teachers (
	id INTEGER NOT NULL,
	dni VARCHAR(10) NOT NULL UNIQUE,
	name VARCHAR(255) NOT NULL,
	knowledge_field VARCHAR(255) NOT NULL,
	PRIMARY KEY (id)
);


CREATE TABLE subjects (
	id INTEGER NOT NULL,
	code VARCHAR(10) NOT NULL UNIQUE,
	name VARCHAR(255) NOT NULL,
	grade_id INTEGER NOT NULL,
	PRIMARY KEY (id),
	FOREIGN KEY (grade_id) REFERENCES grades(id) ON DELETE CASCADE
);


CREATE TABLE themes (
	id INTEGER NOT NULL,
	-- since "order" is reserved...
	-- Also this way is more flexible
	priority INTEGER DEFAULT 1,
	name VARCHAR(255) NOT NULL,
	description TEXT NOT NULL,
	subject_id INTEGER NOT NULL,
	PRIMARY KEY (id),
	FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE
);


CREATE TABLE questions (
	id INTEGER NOT NULL,
	statement VARCHAR(255) NOT NULL,
	PRIMARY KEY (id)
);


CREATE TABLE answers (
	id INTEGER NOT NULL,
    title VARCHAR(255) NOT NULL,
    is_correct SMALLINT CHECK (is_correct in (0,1)),
    priority INTEGER DEFAULT 1,
    question_id INTEGER NOT NULL,
	PRIMARY KEY (id),
    FOREIGN KEY (question_id) REFERENCES questions(id) ON DELETE CASCADE
);


CREATE TABLE exams (
	id INTEGER NOT NULL,
    year INTEGER NOT NULL,
    convocatory INTEGER NOT NULL,
    subject_id INTEGER NOT NULL,
	PRIMARY KEY (id),
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE,
    CONSTRAINT unique_exam UNIQUE (subject_id, year, convocatory)
);


CREATE TABLE questions_themes (
	question_id INTEGER NOT NULL,
	theme_id INTEGER NOT NULL,
	FOREIGN KEY (question_id) REFERENCES questions(id) ON DELETE CASCADE,
	FOREIGN KEY (theme_id) REFERENCES themes(id) ON DELETE CASCADE,
    PRIMARY KEY (question_id, theme_id)
);


CREATE TABLE exams_questions (
	question_id INTEGER NOT NULL,
	exam_id INTEGER NOT NULL,
    correct_answer_count INTEGER DEFAULT 0,
    incorrect_answer_count INTEGER DEFAULT 0,
    unreplied_answer_count INTEGER DEFAULT 0,
	FOREIGN KEY (question_id) REFERENCES questions(id) ON DELETE CASCADE,
	FOREIGN KEY (exam_id) REFERENCES exams(id) ON DELETE CASCADE,
    PRIMARY KEY (question_id, exam_id)
);


CREATE TABLE subjects_teachers (
	subject_id INTEGER NOT NULL,
	teacher_id INTEGER NOT NULL,
	FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE,
	FOREIGN KEY (teacher_id) REFERENCES teachers(id) ON DELETE CASCADE,
    PRIMARY KEY (subject_id, teacher_id)
);
