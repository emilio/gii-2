ALTER TABLE questions ADD exam_count INTEGER DEFAULT 0;

CREATE OR REPLACE TRIGGER question_exam_insert AFTER INSERT ON exams_questions
FOR EACH ROW
BEGIN
	UPDATE questions SET exam_count = exam_count + 1 WHERE id = :new.question_id;
END;
/

CREATE OR REPLACE TRIGGER question_exam_delete AFTER DELETE ON exams_questions
FOR EACH ROW
BEGIN
	UPDATE questions SET exam_count = exam_count - 1 WHERE id = :old.question_id;
END;
/

CREATE OR REPLACE TRIGGER question_exam_insert AFTER UPDATE OF question_id ON exams_questions
FOR EACH ROW
BEGIN
    UPDATE questions SET exam_count = exam_count - 1 WHERE id = :old.question_id;
	UPDATE questions SET exam_count = exam_count + 1 WHERE id = :new.question_id;
END;
/

