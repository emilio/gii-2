USE `exams`;
INSERT INTO `grades` (`name`) VALUES
	('Ingeniería Informática'),
	('Medicina'),
	('Física'),
	('Matemáticas');

INSERT INTO `teachers` (`dni`, `name`, `knowledge_field`) VALUES
	('70912324N', 'Emilio Cobos Álvarez', 'Ingeniería del software (becario)'), /** Nah, es broma ;) */
	('80088399N', 'Pepe Pérez', 'Informática y Automática');

INSERT INTO `subjects` 	(`code`, `name`, `grade_id`) VALUES
	('10011', 'Álgebra Lineal', 1),
	('10110', 'Bases de Datos', 1),
	('11011', 'Estructuras de Datos y Algoritmos', 1);

INSERT INTO `subject_teachers` (`subject_id`, `teacher_id`) VALUES
	(1, 1), 
	(2, 1),
	(3, 2),
	(2, 2);

INSERT INTO `themes` (`order`,	`name`,	`description`, `subject_id`) VALUES
	(1, 'Espacio Vectorial', '', 1),
	(1, 'Introducción a los sistemas de Bases de Datos', 'No quiero inventarme descripciones...', 2),
	(2, 'El Modelo Relacional', '', 2),
	(1, 'Algoritmos de ordenación', '', 3);

INSERT INTO `asks` (`statement`) VALUES
	('La FNBC se caracteriza por...');

INSERT INTO `ask_themes` (`ask_id`, `theme_id`) VALUES (1, 2);

INSERT INTO `answers` (`title`,`is_correct`,`order`,`ask_id`) VALUES
	('No tener dependencias parciales', 0, 0, 1),
	('No tener dependencias transitivas', 0, 1, 1),
	('Tener como únicos determinantes claves candidatas', 0, 2, 1),
	('Todas las anteriores', 0, 3, 1);

INSERT INTO `exams` (`year`, `convocatory`, `subject_id`) VALUES
	(2015, 1, 2);

INSERT INTO `ask_exams` (`ask_id`, `exam_id`) VALUES (1, 1);

