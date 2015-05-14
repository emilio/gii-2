--View 'questions_counter'
CREATE OR REPLACE VIEW questions_counter AS
    SELECT
        teachers.id AS teacher_id,
        teachers.dni AS dni,
        themes.subject_id,
        themes.id AS theme_id,
        -- We want to count all the themes, not just those who have at least one question
        (SELECT COUNT(qt.question_id)
            FROM questions_themes qt
            WHERE qt.theme_id = themes.id) AS question_count
    FROM teachers, subjects_teachers, themes
    WHERE
        teachers.id = subjects_teachers.teacher_id AND
        subjects_teachers.subject_id = themes.subject_id AND
        teachers.dni = USER
    GROUP BY teachers.id, teachers.dni, themes.subject_id, themes.id;

GRANT ALL PRIVILEGES ON questions_counter TO PUBLIC;
