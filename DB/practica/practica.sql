/**
 * Teachers
 */
CREATE TABLE `teachers` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
	`dni` VARCHAR(10) NOT NULL,
	`name` VARCHAR(255) NOT NULL,
	`knowledge_field` VARCHAR(255) NOT NULL,
	UNIQUE KEY (`dni`)
);

/**
 * Grades
 */
CREATE TABLE `grades` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
	`name` VARCHAR(255)
);

/**
 * Subjects
 */
CREATE TABLE `subjects` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
	`code` VARCHAR(10) NOT NULL,
	`name` VARCHAR(255) NOT NULL,
	`grade_id` BIGINT UNSIGNED /* NOT NULL */,
	UNIQUE KEY (`code`),
	FOREIGN KEY (`grade_id`) REFERENCES `grades`(`id`) ON DELETE SET NULL
);

/**
 * Relation subject_teachers
 */
CREATE TABLE `subject_teachers` (
	`subject_id` BIGINT UNSIGNED NOT NULL,
	`teacher_id` BIGINT UNSIGNED NOT NULL,
	FOREIGN KEY (`subject_id`) REFERENCES `subjects`(`id`) ON DELETE CASCADE,
	FOREIGN KEY (`teacher_id`) REFERENCES `teachers`(`id`) ON DELETE CASCADE,
);

/**
 * Themes
 */
CREATE TABLE `themes` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
	`order` INT NOT NULL DEFAULT 1,
	`title` VARCHAR(255) NOT NULL,
	`description` TEXT,
	`subject_id` BIGINT UNSIGNED NOT NULL,
	FOREIGN KEY (`subject_id`) REFERENCES `subjects`(`id`) ON DELETE CASCADE,
	UNIQUE KEY (`subject_id`, `order`)
);

/**
 * Asks
 */
CREATE TABLE `asks` (
	`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
	`statement` VARCHAR(255) NOT NULL
);
