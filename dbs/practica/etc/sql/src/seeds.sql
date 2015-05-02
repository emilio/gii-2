--Table "grades"
INSERT INTO grades VALUES(0001, 'Grado en Ingeniería Informática');
INSERT INTO grades VALUES(0002, 'Ingeniería Técnica en Informática de Gestión');
INSERT INTO grades VALUES(0003, 'Grado en Ingeniería Informática en Sistemas de Información');
INSERT INTO grades VALUES(0004, 'Ingeniería Técnica en Informática de Sistemas');
INSERT INTO grades VALUES(0005, 'Grado en Ingeniería del Software');



--Table "teachers"
INSERT INTO teachers VALUES(0001, '70015789L', 'Jacinto Pérez Hermoso', 'Automática');
INSERT INTO teachers VALUES(0002, '71085749F', 'Brígida Ferreira Santos', 'Robótica');
INSERT INTO teachers VALUES(0003, '78014787Y', 'Tomás Fernández Marín', 'Inteligencia Artifical');
INSERT INTO teachers VALUES(0004, '70032427T', 'Laura Galende Hernández', 'Informática');
INSERT INTO teachers VALUES(0005, '70011497H', 'José de Pedro Santos', 'Nanotecnología');



--Table "subjects"
INSERT INTO subjects VALUES(00010, '00015A', 'Algoritmia', 0001);
INSERT INTO subjects VALUES(00020, '00128E', 'Estructuras de Datos', 0002);
INSERT INTO subjects VALUES(00030, '00021I', 'Informática Teórica', 0003);
INSERT INTO subjects VALUES(00040, '00052O', 'Organización y gestión de empresas', 0004);
INSERT INTO subjects VALUES(00050, '00073U', 'Sistemas Operativos', 0005);



--Table "themes"
INSERT INTO themes VALUES(0001, 1, 'Algoritmos de búsqueda y ordenación', 'Búsqueda de un elemento específico dentro de un conjunto cualquiera de datos, y ordenación del mismo.', 00010);
INSERT INTO themes VALUES(0002, 1, 'Tipos Abstractos de Datos (TAD)', 'Herramienta fundamental de la programación que generaliza un operador y encapsula un programa.', 00020);
INSERT INTO themes VALUES(0003, 1, 'Autómatas Finitos', 'Modelo computacional que realiza cómputos en forma automática sobre una entrada para producir una salida.', 00030);
INSERT INTO themes VALUES(0004, 1, 'Análisis de costo-beneficio', 'Es una lógica o razonamiento basado en el principio de obtener los mayores y mejores resultados al menor esfuerzo invertido.', 00040);
INSERT INTO themes VALUES(0005, 1, 'Planificación del procesador', 'Mecanismos más comunes que poseen los sistemas operativos actuales para realizar la gestión del procesador.', 00050);



--Table "questions"
INSERT INTO questions VALUES(0001, '¿Cuál de estas funciones representa una cota superior asintótica para el archiconocido algoritmo de ordenación QuickSort?');
INSERT INTO questions VALUES(0002, 'Indica cuál de los siguientes TAD sigue un esquema LIFO (Last In, First Out):');
INSERT INTO questions VALUES(0003, '¿Qué es un AFD?');
INSERT INTO questions VALUES(0004, 'Señala cuál de las siguientes afirmaciones sobre las empresas es correcta:');
INSERT INTO questions VALUES(0005, '¿En qué consiste la planificación del procesador?');



--Table "answers"
INSERT INTO answers VALUES(000100, 'A) nlog(n)', 1, 1, 0001);
INSERT INTO answers VALUES(000101, 'B) n^2', 0, 2, 0001);
INSERT INTO answers VALUES(000102, 'C) n', 0, 3, 0001);
INSERT INTO answers VALUES(000103, 'D) n^3', 0, 4, 0001);
-------------------------------------------------------------------------------------------------------------
INSERT INTO answers VALUES(000200, 'A) Colas', 0, 1, 0002);
INSERT INTO answers VALUES(000201, 'B) Pilas', 1, 2, 0002);
INSERT INTO answers VALUES(000202, 'C) Grafos', 0, 3, 0002);
INSERT INTO answers VALUES(000203, 'D) Conjuntos Disjuntos', 0, 4, 0002);
-------------------------------------------------------------------------------------------------------------
INSERT INTO answers VALUES(000300, 'A) Autómata Finito No Determinsita', 0, 1, 0003);
INSERT INTO answers VALUES(000301, 'B) Máquina de Turing', 0, 2, 0003);
INSERT INTO answers VALUES(000302, 'C) Autómata Finito Determinista', 1, 3, 0003);
INSERT INTO answers VALUES(000303, 'D) Conjunto Universal', 0, 4, 0003);
-------------------------------------------------------------------------------------------------------------
INSERT INTO answers VALUES(000400, 'A) Debe de maximizar beneficios minimizando costes', 1, 1, 0004);
INSERT INTO answers VALUES(000401, 'B) Debe de minimizar beneficios maximizando costes', 0, 2, 0004);
INSERT INTO answers VALUES(000402, 'C) Debe de maximizar beneficios maximizando costes', 0, 3, 0004);
INSERT INTO answers VALUES(000403, 'D) Debe de minimizar beneficios minimizando costes', 0, 4, 0004);
-------------------------------------------------------------------------------------------------------------
INSERT INTO answers VALUES(000500, 'A) Formatear los datos contenidos en un Disco Duro', 0, 1, 0005);
INSERT INTO answers VALUES(000501, 'B) Varias personas utilizando un mismo Sistema a la vez', 0, 2, 0005);
INSERT INTO answers VALUES(000502, 'C) Gestión de los mecanismos IPC del Sistema Operativo', 0, 3, 0005);
INSERT INTO answers VALUES(000503, 'D) Dar un buen servicio a los procesos de un Sistema', 1, 4, 0005);



--Table "exams"
INSERT INTO exams VALUES(0001, 2007, 1, 00010);
INSERT INTO exams VALUES(0002, 2008, 2, 00020);
INSERT INTO exams VALUES(0003, 2005, 1, 00030);
INSERT INTO exams VALUES(0004, 2010, 1, 00040);
INSERT INTO exams VALUES(0005, 2015, 2, 00050);



--Table "questions_themes"
INSERT INTO questions_themes VALUES(0001, 0001);
INSERT INTO questions_themes VALUES(0002, 0002);
INSERT INTO questions_themes VALUES(0003, 0003);
INSERT INTO questions_themes VALUES(0004, 0004);
INSERT INTO questions_themes VALUES(0005, 0005);



--Table "exams_questions"
INSERT INTO exams_questions VALUES(0001, 0001);
INSERT INTO exams_questions VALUES(0002, 0002);
INSERT INTO exams_questions VALUES(0003, 0003);
INSERT INTO exams_questions VALUES(0004, 0004);
INSERT INTO exams_questions VALUES(0005, 0005);



--Table "subjects_teachers"
INSERT INTO subjects_teachers VALUES(00010, 0001);
INSERT INTO subjects_teachers VALUES(00020, 0002);
INSERT INTO subjects_teachers VALUES(00030, 0003);
INSERT INTO subjects_teachers VALUES(00040, 0004);
INSERT INTO subjects_teachers VALUES(00050, 0005);
