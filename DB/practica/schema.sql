CREATE DATABASE exams
 DEFAULT CHARACTER SET utf8
 DEFAULT COLLATE utf8_general_ci;

USE exams;

/** Grades */
CREATE TABLE `grades` (
	`id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
	`name` VARCHAR(255) NOT NULL
) ENGINE = INNODB;

/** Teachers */
CREATE TABLE `teachers` (
	`id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,

	/** Nótese que el campo dni estaría mejor almacenado como un int 
	    y comprobándose con las reglas conocidas... Pero... */
	`dni` VARCHAR(10) NOT NULL,
	`name` VARCHAR(255) NOT NULL,
	`knowledge_field` VARCHAR(255) NOT NULL,
	UNIQUE KEY (`dni`)
) ENGINE = INNODB;

/** Subjects */
CREATE TABLE `subjects` (
	`id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
	`code` VARCHAR(10) NOT NULL,
	`name` VARCHAR(255) NOT NULL,
	`grade_id` INT UNSIGNED NOT NULL,
	UNIQUE KEY (`code`),
	FOREIGN KEY (`grade_id`) REFERENCES `grades`(`id`) ON DELETE CASCADE
) ENGINE = INNODB;

/** Themes */
CREATE TABLE `themes` (
	`id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
	`order` INT NOT NULL DEFAULT 1,
	`name` VARCHAR(255) NOT NULL,
	`description` TEXT NOT NULL,
	`subject_id` INT UNSIGNED NOT NULL,
	FOREIGN KEY (`subject_id`) REFERENCES `subjects`(`id`) ON DELETE CASCADE,
	UNIQUE KEY (`subject_id`, `order`)
) ENGINE = INNODB;

/** Asks */
CREATE TABLE `asks` (
	`id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
	`statement` VARCHAR(255) NOT NULL
) ENGINE = INNODB;

/** Answers */
CREATE TABLE `answers` (
	`id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `title` VARCHAR(255) NOT NULL,
    `is_correct` BOOLEAN NOT NULL DEFAULT 0,
    `order` INT UNSIGNED NOT NULL,
    `ask_id` INT UNSIGNED NOT NULL,
    FOREIGN KEY (`ask_id`) REFERENCES `asks`(`id`) ON DELETE CASCADE,
    UNIQUE KEY (`ask_id`, `order`)
) ENGINE = INNODB;

/** Exams */
CREATE TABLE `exams` (
	`id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `year` INT UNSIGNED NOT NULL,
    `convocatory` INT UNSIGNED NOT NULL,
    `subject_id` INT UNSIGNED NOT NULL,
    FOREIGN KEY (`subject_id`) REFERENCES `subjects`(`id`) ON DELETE CASCADE,
    UNIQUE KEY (`subject_id`, `year`, `convocatory`)
) ENGINE = INNODB;

/** Relation ask_themes */
CREATE TABLE `ask_themes` (
	`ask_id` INT UNSIGNED NOT NULL,
	`theme_id` INT UNSIGNED NOT NULL,
	FOREIGN KEY (`ask_id`) REFERENCES `asks`(`id`) ON DELETE CASCADE,
	FOREIGN KEY (`theme_id`) REFERENCES `themes`(`id`) ON DELETE CASCADE,
    PRIMARY KEY (`ask_id`, `theme_id`)
) ENGINE = INNODB;

/** Relation ask_exams */
CREATE TABLE `ask_exams` (
	`ask_id` INT UNSIGNED NOT NULL,
	`exam_id` INT UNSIGNED NOT NULL,
	FOREIGN KEY (`ask_id`) REFERENCES `asks`(`id`) ON DELETE CASCADE,
	FOREIGN KEY (`exam_id`) REFERENCES `exams`(`id`) ON DELETE CASCADE,
    PRIMARY KEY (`ask_id`, `exam_id`)
) ENGINE = INNODB;

/** Relation subject_teachers */
CREATE TABLE `subject_teachers` (
	`subject_id` INT UNSIGNED NOT NULL,
	`teacher_id` INT UNSIGNED NOT NULL,
	FOREIGN KEY (`subject_id`) REFERENCES `subjects`(`id`) ON DELETE CASCADE,
	FOREIGN KEY (`teacher_id`) REFERENCES `teachers`(`id`) ON DELETE CASCADE,
    PRIMARY KEY (`subject_id`, `teacher_id`)
) ENGINE = INNODB;
