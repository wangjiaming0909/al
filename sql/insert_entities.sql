--department
INSERT INTO department(dept_name, building, budget)
	values('Biology', 'Watson', 90000);
INSERT INTO department(dept_name, building, budget)
	values('Comp.Sci.', 'Taylor', 100000);
INSERT INTO department(dept_name, building, budget)
	values('Elec.Eng.', 'Taylor', 85000);
INSERT INTO department(dept_name, building, budget)
	values('Finance', 'Painter', 120000);
INSERT INTO department(dept_name, building, budget)
	values('History', 'Painter', 50000);
INSERT INTO department(dept_name, building, budget)
	values('Music', 'Packard', 80000);
INSERT INTO department(dept_name, building, budget)
	values('Physics', 'Watson', 70000);

--department

--course
INSERT INTO course(course_id, title, dept_name, credits) 
	values('CS-101', 'Intro. to Comp Sci', 'Comp.Sci.', 4);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('CS-437', 'Comp. Sci. System', 'Comp.Sci.', 4);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('CS-190', 'Game Design', 'Comp.Sci.', 3);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('CS-315', 'Image Processing', 'Comp.Sci.', 4);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('CS-319', 'Robotics', 'Comp.Sci.', 4);

INSERT INTO course(course_id, title, dept_name, credits) 
	values('BIO-101', 'Intro. to Biology', 'Biology', 4);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('BIO-301', 'Genetics', 'Biology', 4);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('BIO-399', 'Computational Biology', 'Biology', 3);

INSERT INTO course(course_id, title, dept_name, credits) 
	values('EE-181', 'Intro. to Digital Systems', 'Elec.Eng.', 3);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('FIN-201', 'Investment Banking', 'Finance', 3);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('HIS-351', 'World history', 'History', 3);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('MU-199', 'Music Video Production', 'Music', 3);
INSERT INTO course(course_id, title, dept_name, credits) 
	values('PHY-101', 'Physical Principles', 'Physics', 4);
--course


--instructor
INSERT INTO instructor(ID, name, dept_name, salary)
	values('10101', 'Srinivasan', 'Comp.Sci.', 65000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('12121', 'Wu', 'Finance', 90000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('15151', 'WANG', 'Music', 60000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('22222', 'Jiaming', 'Physics', 40000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('12342', 'He', 'History', 50000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('65123', 'Lemei', 'Physics', 45000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('12564', 'Peng', 'Comp.Sci.', 90000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('87694', 'Zuolei', 'History', 68000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('12349', 'Fu', 'Biology', 50000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('59435', 'Yong', 'Finance', 50000);
INSERT INTO instructor(ID, name, dept_name, salary)
	values('46897', 'Ming', 'Elec.Eng.', 96000);
--instructor

--section
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('BIO-101', 1, 'Summer', 2009, 'Painter', '514', 'B');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('BIO-301', 1, 'Summer', 2010, 'Painter', '514', 'A');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('CS-101', 1, 'Fall', 2009, 'Packard', '101', 'H');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('CS-101', 1, 'Spring', 2010, 'Packard', '101', 'F');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('CS-190', 1, 'Spring', 2009, 'Taylor', '3128', 'E');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('CS-190', 2, 'Spring', 2009, 'Taylor', '3128', 'A');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('CS-315', 1, 'Spring', 2010, 'Watson', '120', 'D');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('CS-319', 1, 'Spring', 2010, 'Watson', '1-00', 'B');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('CS-319', 2, 'Spring', 2010, 'Taylor', '3128', 'C');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('CS-437', 1, 'Fall', 2009, 'Taylor', '3128', 'A');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('EE-181', 1, 'Spring', 2009, 'Taylor', '3128', 'C');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('FIN-201', 1, 'Spring', 2010, 'Packard', '101', 'B');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('HIS-351', 1, 'Spring', 2010, 'Painter', '514', 'C');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('MU-199', 1, 'Spring', 2010, 'Packard', '101', 'D');
INSERT INTO section(course_id, sec_id, semester, year_, building, room_number, time_slot_id)
	values('PHY-101', 1, 'Fall', 2009, 'Watson', '100', 'A');
--section

--teaches
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('10101', 'CS-101', '1', 'Fall', 2009);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('10101', 'CS-315', '1', 'Spring', 2010);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('10101', 'CS-437', '1', 'Fall', 2009);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('12121', 'FIN-201', '1', 'Spring', 2010);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('15151', 'MU-199', '1', 'Spring', 2010);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('22222', 'PHY-101', '1', 'Fall', 2009);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('12342', 'HIS-351', '1', 'Spring', 2010);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('12564', 'CS-101', '1', 'Spring', 2010);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('12564', 'CS-319', '1', 'Spring', 2010);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('12349', 'BIO-101', '1', 'Summer', 2009);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('12349', 'BIO-301', '1', 'Summer', 2010);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('10101', 'CS-190', '1', 'Spring', 2009);
--INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	--values('10101', 'CS-390', '2', 'Spring', 2009);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('10101', 'CS-319', '2', 'Spring', 2010);
INSERT INTO teaches (ID, course_id, sec_id, semester, year_)
	values('46897', 'EE-181', '1', 'Spring', 2009);

--teaches

--student
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('00128', 'Zhang', 'Comp.Sci.', 102);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('12345', 'Shankar', 'Comp.Sci.', 32);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('19991', 'Brandt', 'History', 80);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('23121', 'Chavez', 'Finance', 110);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('44553', 'Peltier', 'Physics', 56);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('45678', 'Levy', 'Physics', 46);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('54321', 'Williams', 'Comp.Sci.', 54);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('55739', 'Sanchez', 'Music', 38);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('70557', 'Snow', 'Physics', 0);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('76543', 'Brown', 'Comp.Sci.', 58);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('76653', 'Aoi', 'Elec.Eng.', 60);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('98765', 'Bourikas', 'Elec.Eng.', 98);
INSERT INTO student (ID, name, dept_name, tot_cred)
	VALUES ('98988', 'Tanaka', 'Biology', 120);
--student

--takes
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('00128', 'CS-101', '1', 'Fall', 2009, 'A');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('00128', 'CS-437', '1', 'Fall', 2009, 'A-');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('12345', 'CS-101', '1', 'Fall', 2009, 'C');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('12345', 'CS-190', '2', 'Spring', 2009, 'A');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('12345', 'CS-315', '1', 'Spring', 2010, 'A');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('12345', 'CS-437', '1', 'Fall', 2009, 'A');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('19991', 'HIS-351', '1', 'Spring', 2010, 'B');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('23121', 'FIN-201', '1', 'Spring', 2010, 'C+');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('44553', 'PHY-101', '1', 'Fall', 2009, 'B-');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('45678', 'CS-101', '1', 'Fall', 2009, 'F');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('45678', 'CS-101', '1', 'Spring', 2010, 'B+');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('45678', 'CS-319', '1', 'Spring', 2010, 'B');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('54321', 'CS-101', '1', 'Fall', 2009, 'A-');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('54321', 'CS-190', '2', 'Spring', 2009, 'B+');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('55739', 'MU-199', '1', 'Spring', 2010, 'A-');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('76543', 'CS-101', '1', 'Fall', 2009, 'A');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('76543', 'CS-319', '2', 'Spring', 2010, 'A');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('76653', 'EE-181', '1', 'Spring', 2009, 'C');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('98765', 'CS-101', '1', 'Fall', 2009, 'C-');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('98765', 'CS-315', '1', 'Spring', 2010, 'B');
INSERT INTO takes (ID, course_id, sec_id, semester, year_, grade)
	VALUES('98988', 'BIO-101', '1', 'Summer', 2009, 'A');
INSERT INTO takes (ID, course_id, sec_id, semester, year_)
	VALUES('98988', 'BIO-301', '1', 'Summer', 2010);
--takes

