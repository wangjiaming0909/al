USE school;




-- SELECT name, course_id FROM student, takes WHERE student.ID = takes.ID;
-- SELECT name, course_id FROM student join takes on student.ID = takes.ID; 

-- SELECT * FROM student, takes WHERE student.ID = takes.ID;
-- SELECT * FROM student join takes on student.ID = takes.ID; 
-- SELECT * FROM student join takes USING (ID); 
-- SELECT * FROM student natural join takes; 
-- SELECT * FROM student natural left outer join takes;
-- SELECT name, ID FROM student natural left outer join takes where course_id is null;
-- SELECT * FROM takes natural right outer join student;
-- SELECT * FROM student left outer join takes on student.ID = takes.ID; /*with snow*/
-- SELECT * FROM student left outer join takes on true WHERE student.ID = takes.ID; /*no snow*/


SELECT * -- a.ID, a.tot_cred, count(distinct b.tot_cred), a.name
FROM student a, student b
WHERE b.tot_cred >= a.tot_cred -- and a.name = 'Tanaka'
-- group by a.ID 
order by a.tot_cred desc



