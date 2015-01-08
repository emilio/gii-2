/** Teachers */
CREATE TABLE `teachers` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
	`dni` VARCHAR(10) NOT NULL,
	`name` VARCHAR(255) NOT NULL,
	`knowledge_field` VARCHAR(255) NOT NULL,
	PRIMARY KEY (`id`),
	UNIQUE KEY (`dni`)
);

/** Grades */
CREATE TABLE `grades` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(255),
	PRIMARY KEY (`id`)
);

/** Subjects */
CREATE TABLE `subjects` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
	`code` VARCHAR(10) NOT NULL,
	`name` VARCHAR(255) NOT NULL,
	`grade_id` BIGINT UNSIGNED /* NOT NULL */,
	PRIMARY KEY (`id`),
	UNIQUE KEY (`code`),
	FOREIGN KEY (`grade_id`) REFERENCES `grades`(`id`) ON DELETE SET NULL
);

/** Relation subject_teachers */
CREATE TABLE `subject_teachers` (
	`subject_id` BIGINT UNSIGNED NOT NULL,
	`teacher_id` BIGINT UNSIGNED NOT NULL,
	PRIMARY KEY (`subject_id`, `teacher_id`),
	FOREIGN KEY (`subject_id`) REFERENCES `subjects`(`id`) ON DELETE CASCADE,
	FOREIGN KEY (`teacher_id`) REFERENCES `teachers`(`id`) ON DELETE CASCADE,
);

/** Themes */
CREATE TABLE `themes` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
	`order` INT NOT NULL DEFAULT 1,
	`name` VARCHAR(255) NOT NULL,
	`description` TEXT,
	`subject_id` BIGINT UNSIGNED NOT NULL,
	PRIMARY KEY (`id`),
	FOREIGN KEY (`subject_id`) REFERENCES `subjects`(`id`) ON DELETE CASCADE,
	UNIQUE KEY (`subject_id`, `order`)
);

/** Asks */
CREATE TABLE `asks` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
	`statement` VARCHAR(255) NOT NULL,
	PRIMARY KEY (`id`)
);


/** An ask is useful to evaluate one or various themes */
CREATE TABLE `ask_themes` (
	`ask_id` BIGINT UNSIGNED NOT NULL,
	`theme_id` BIGINT UNSIGNED NOT NULL,
	PRIMARY KEY (`ask_id`, `theme_id`),
	FOREIGN KEY (`ask_id`) REFERENCES `asks`(`id`) ON DELETE CASCADE,
	FOREIGN KEY (`theme_id`) REFERENCES `themes`(`id`) ON DELETE CASCADE
);

/** Answers */
CREATE TABLE `answers` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
	`ask_id` BIGINT UNSIGNED NOT NULL,
	`title` VARCHAR(255) NOT NULL,
	`is_correct` BOOLEAN NOT NULL DEFAULT 0,
	`order` INT NOT NULL DEFAULT 1,
	PRIMARY KEY (`id`),
	FOREIGN KEY (`ask_id`) REFERENCES `asks`(`id`) ON DELETE CASCADE,
	UNIQUE KEY (`ask_id`, `order`)
);

/** Exams */
CREATE TABLE `exams` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
	`subject_id` BIGINT UNSIGNED NOT NULL,
	`year` YEAR(4) NOT NULL,
	`convocatory` INT UNSIGNED NOT NULL,
	PRIMARY KEY (`id`),
	UNIQUE KEY (`year`, `convocatory`)
);

/** An ask can be useful in multiple exams */
CREATE TABLE `ask_exams` (
	`ask_id` BIGINT UNSIGNED NOT NULL,
	`exam_id` BIGINT UNSIGNED NOT NULL,
	PRIMARY KEY (`ask_id`, `exam_id`),
	FOREIGN KEY (`ask_id`) REFERENCES `asks`(`id`) ON DELETE CASCADE,
	FOREIGN KEY (`exam_id`) REFERENCES `exams`(`id`) ON DELETE CASCADE
);
