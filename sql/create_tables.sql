use school;
CREATE TABLE department(
	dept_name varchar(20),
	building varchar(15),
	budget numeric(12,2),
	PRIMARY KEY (dept_name));
	
CREATE TABLE course(
	course_id varchar(7),
	title varchar(50),
	dept_name varchar(20),
	credits numeric(2,0),
	PRIMARY KEY (course_id),
	FOREIGN KEY (dept_name) REFERENCES department(dept_name));

CREATE TABLE instructor(
	ID varchar(5),
	name varchar(20) NOT NULL,
	dept_name varchar(20),
	salary numeric(8,2),
	PRIMARY KEY (ID),
	FOREIGN KEY (dept_name) REFERENCES department(dept_name));
	
CREATE TABLE section(
	course_id varchar(8),
	sec_id varchar(8),
	semester varchar(6),
	year_ numeric(4,0),
	building varchar(15),
	room_number varchar(7),
	time_slot_id varchar(4),
	PRIMARY KEY (course_id, sec_id, semester, year_),
	FOREIGN KEY (course_id) REFERENCES course(course_id));
	
CREATE TABLE teaches(
	ID varchar(5),
	course_id varchar(8),
	sec_id varchar(8),
	semester varchar(6),
	year_ numeric(4,0),
	PRIMARY KEY (ID, course_id, semester, year_),
	FOREIGN KEY (course_id, sec_id, semester, year_)
		REFERENCES section(course_id, sec_id, semester, year_),
	FOREIGN KEY (ID) REFERENCES instructor(ID));

CREATE TABLE student(
	ID VARCHAR(5),
	name varchar(20) NOT NULL,
	dept_name varchar(20),
	tot_cred NUMERIC(4,0),
	PRIMARY KEY (ID),
	FOREIGN KEY (dept_name) REFERENCES department(dept_name));

CREATE TABLE takes(
	ID VARCHAR(5),
	course_id VARCHAR(8),
	sec_id VARCHAR(8),
	semester VARCHAR(6),
	year_ NUMERIC(4, 0),
	grade VARCHAR(4),
	PRIMARY KEY (ID, course_id, semester, year_),
	FOREIGN KEY (course_id, sec_id, semester, year_)
		REFERENCES section(course_id, sec_id, semester, year_),
	FOREIGN KEY (ID) REFERENCES student(ID));
