ALTER TABLE questions ADD exam_count INTEGER DEFAULT 0;
CREATE TRIGGER question_exam_insert AFTER INSERT ON exams_questions
FOR EACH ROW
BEGIN
	UPDATE questions SET exam_count = exam_count + 1 WHERE id = :new.question_id;
END;
/

CREATE TRIGGER question_exam_delete AFTER DELETE ON exams_questions
FOR EACH ROW
BEGIN
	UPDATE questions SET exam_count = exam_count - 1 WHERE id = :old.question_id
END;
/

CREATE TRIGGER question_exam_insert AFTER UPDATE OF question_id ON exams_questions
FOR EACH ROW
BEGIN
    UPDATE questions SET exam_count = exam_count - 1 WHERE id = :old.question_id;
	UPDATE questions SET exam_count = exam_count + 1 WHERE id = :new.question_id;
END;
/


--View 'questions_counter'
CREATE OR REPLACE VIEW questions_counter
AS SELECT teacher_id, s.subject_id, count (question_id) as total_number, theme_id, t.subject_id, id
FROM subjects_teachers s, questions_themes, themes t
WHERE teacher_id=USER and theme_id=id and t.subject_id=s.subject_id;
GRANT all privileges
ON questions_counter
to public;
