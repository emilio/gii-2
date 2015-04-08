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
