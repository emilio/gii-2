--Table "grades"
INSERT INTO grades VALUES(1, 'Grado en Ingeniería Informática');
INSERT INTO grades VALUES(2, 'Ingeniería Técnica en Informática de Gestión');
INSERT INTO grades VALUES(3, 'Grado en Ingeniería Informática en Sistemas de Información');
INSERT INTO grades VALUES(4, 'Ingeniería Técnica en Informática de Sistemas');
INSERT INTO grades VALUES(5, 'Grado en Ingeniería del Software');


--Table "teachers"
INSERT INTO teachers VALUES(1, '70015789L', 'Jacinto Pérez Hermoso', 'Automática');
INSERT INTO teachers VALUES(2, '71085749F', 'Brígida Ferreira Santos', 'Robótica');
INSERT INTO teachers VALUES(3, '78014787Y', 'Tomás Fernández Marín', 'Inteligencia Artifical');
INSERT INTO teachers VALUES(4, '70032427T', 'Laura Galende Hernández', 'Informática');
INSERT INTO teachers VALUES(5, '70011497H', 'José de Pedro Santos', 'Nanotecnología');
INSERT INTO teachers VALUES(6, 'OPS$I0912324', 'Emilio Cobos Álvarez', 'Teoría de la computación');


--Table "subjects"
INSERT INTO subjects VALUES(1, '00015A', 'Algoritmia', 1);
INSERT INTO subjects VALUES(2, '00128E', 'Estructuras de Datos', 2);
INSERT INTO subjects VALUES(3, '00021I', 'Informática Teórica', 3);
INSERT INTO subjects VALUES(4, '00052O', 'Organización y gestión de empresas', 4);
INSERT INTO subjects VALUES(5, '00073U', 'Sistemas Operativos', 5);


--Table "themes"
--First subject
INSERT INTO themes VALUES(1, 1, 'Algoritmos de ordenación', 'Ordenación de un elemento específico dentro de un conjunto cualquiera de datos.', 1);
INSERT INTO themes VALUES(2, 1, 'Algoritmos de búsqueda', 'Búsqueda de un elemento específico dentro de un conjunto cualquiera de datos.', 1);
INSERT INTO themes VALUES(3, 1, 'Notación asintótica', 'Diferentes métodos para expresar el tiempo de ejecución de un algoritmo.', 1);
INSERT INTO themes VALUES(4, 1, 'Esquemas algorítmicos', 'Agrupación de los algoritmos dentro de distintos esquemas conocidos', 1);
INSERT INTO themes VALUES(5, 1, 'Análisis algorítmico', 'Realizar estimaciones teóricas para los recursos que necesita cualquier algoritmo que resuelva un problema computacional dado.', 1);
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--Second subject
INSERT INTO themes VALUES(6, 1, 'TAD: Pilas', 'Se utilizan generalmente para simplificar ciertas operaciones de programación.', 2);
INSERT INTO themes VALUES(7, 1, 'TAD: Árboles Binarios de búsqueda', 'Ampliación del TAD Árboles Binarios, que nos permiten recuperar una información determinada.', 2);
INSERT INTO themes VALUES(8, 1, 'TAD: Grafos', 'Conjunto de nodos (también llamados vértices) y un conjunto de arcos (aristas) que establecen relaciones entre los nodos.', 2);
INSERT INTO themes VALUES(9, 1, 'TAD: Conjuntos Disjuntos', 'Conjunto basado en la idea de representación de relaciones entre los elementos del conjunto.', 2);
INSERT INTO themes VALUES(10, 1, 'TAD: Montículos Binarios', 'Tipo de implementación de colas de prioridad.', 2);
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--Third subject
INSERT INTO themes VALUES(11, 1, 'Autómatas Finitos', 'Modelos computacionales que realiza cómputos en forma automática sobre una entrada para producir una salida.', 3);
INSERT INTO themes VALUES(12, 1, 'Lenguajes', 'Lo referente a símbolos primitivos y reglas para unir esos símbolos que están formalmente especificados.', 3);
INSERT INTO themes VALUES(13, 1, 'Gramáticas formales', 'Estructuras matemáticas con un conjunto de reglas de formación que definen las cadenas de caracteres admisibles en un determinado lenguaje formal o lengua natural.', 3);
INSERT INTO themes VALUES(14, 1, 'Máquinas abstractas', 'Modelos teóricos de un sistema computador de hardware o software usado en la teoría de autómatas.', 3);
INSERT INTO themes VALUES(15, 1, 'Máquina de Turing', 'Dispositivo que manipula símbolos sobre una tira de cinta de acuerdo a una tabla de reglas.', 3);
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--Fourth subject
INSERT INTO themes VALUES(16, 1, 'Análisis de costo-beneficio', 'Es una lógica o razonamiento basado en el principio de obtener los mayores y mejores resultados al menor esfuerzo invertido.', 4);
INSERT INTO themes VALUES(17, 1, 'La empresa y el sistema económico', 'La empresa como estructura de producción, de asignación de recursos económicos, distribución y consumo de bienes y servicios en una economía.', 4);
INSERT INTO themes VALUES(18, 1, 'La empresa como realidad económica', 'Conjunto ordenado de factores destinados a la producción.', 4);
INSERT INTO themes VALUES(19, 1, 'La empresa como sistema', 'Teoría de sistemas aplicada a la empresa. Comportamiento, procesos de control y adaptación al entorno.', 4);
INSERT INTO themes VALUES(20, 1, 'Inversión y financiación', 'La supervivencia de una empresa depende de su acertada planificación en cuanto a su política de inversiones, financiaciones y la continuidad de éstas en el tiempo.', 4);
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--Fifth subject
INSERT INTO themes VALUES(21, 1, 'Planificación del procesador', 'Mecanismos más comunes que poseen los sistemas operativos actuales para realizar la gestión del procesador.', 5);
INSERT INTO themes VALUES(22, 1, 'Introducción a los Sistemas Operativos', 'Funciones, objetivos, evolución y tipos, así como componentes y estructura de un Sistema Operativo.', 5);
INSERT INTO themes VALUES(23, 1, 'Descripción y control de procesos', 'Estados, implementación y control de los procesos.', 5);
INSERT INTO themes VALUES(24, 1, 'Gestión de la memoria principal', 'Conceptos fundamentales, asignación contigua de memoria, paginación y segmentación.', 5);
INSERT INTO themes VALUES(25, 1, 'Gestón de la memoria virtual', 'Paginación bajo demanda, prepaginación, reemplazo de páginas, gestión del conjunto residente, control de carga y ejemplos de gestión de memoria.', 5);


--Table "questions"
--First subject
INSERT INTO questions (id, statement) VALUES(1, '¿Cuál de estas funciones representa una cota superior asintótica para el archiconocido algoritmo de ordenación QuickSort?');
INSERT INTO questions (id, statement) VALUES(2, '¿Cuál de estas funciones representa una cota superior asintótica para el archiconocido algoritmo de búsqueda Burbuja');
INSERT INTO questions (id, statement) VALUES(3, '¿Qué métodos diferentes de notaciones asintóticas hemos estudiado en clase?');
INSERT INTO questions (id, statement) VALUES(4, '¿A qué esquema algorítmico pertenece el famoso problema de ajedrez de Las Ocho Reinas?');
INSERT INTO questions (id, statement) VALUES(5, '¿Con qué sencilla fórmula matemática se puede analizar un bucle for?');
----------------------------------------------------------------------------------------------------------------------------------------------------------------
--Second subject
INSERT INTO questions (id, statement) VALUES(6, 'Indica qué tipo de esquema sigue este TAD:');
INSERT INTO questions (id, statement) VALUES(7, '¿En qué se diferencia este TAD de los Árboles Binarios normales?');
INSERT INTO questions (id, statement) VALUES(8, '¿Qué es un Árbol de Expansión mínima?');
INSERT INTO questions (id, statement) VALUES(9, '¿Mediante qué tipo de estructuras se pueden implementar un Conjunto Disjunto?');
INSERT INTO questions (id, statement) VALUES(10, '¿Qué dos propiedades rigen cualquier Montículo Binario?');
----------------------------------------------------------------------------------------------------------------------------------------------------------------
--Third subject
INSERT INTO questions (id, statement) VALUES(11, '¿Qué es un AFD?');
INSERT INTO questions (id, statement) VALUES(12, '¿Qué es un lenguaje?');
INSERT INTO questions (id, statement) VALUES(13, '¿Qué es una gramática formal?');
INSERT INTO questions (id, statement) VALUES(14, '¿Para qué son usadas las máquinas abstractas?');
INSERT INTO questions (id, statement) VALUES(15, 'Sobre la Máquina de Turing, señala la afirmación correcta:');
----------------------------------------------------------------------------------------------------------------------------------------------------------------
--Fourth subject
INSERT INTO questions (id, statement) VALUES(16, 'Señala cuál de las siguientes afirmaciones sobre las empresas es correcta:');
INSERT INTO questions (id, statement) VALUES(17, 'Señala la verdadera:');
INSERT INTO questions (id, statement) VALUES(18, '¿Cuáles son las funciones de la empresa dentro de una economía de mercado?');
INSERT INTO questions (id, statement) VALUES(19, '¿Cuáles son los principios básicos de la Teoría de Sistemas?');
INSERT INTO questions (id, statement) VALUES(20, '¿Cuál es el objetivo único de la inversión?');
----------------------------------------------------------------------------------------------------------------------------------------------------------------
--Fivth subject
INSERT INTO questions (id, statement) VALUES(21, '¿En qué consiste la planificación del procesador?');
INSERT INTO questions (id, statement) VALUES(22, '¿Cuáles son las funciones de un Sistema Operativo?');
INSERT INTO questions (id, statement) VALUES(23, '¿Qué es un proceso?');
INSERT INTO questions (id, statement) VALUES(24, '¿Qué problemas puede ocasionar la multiprogramación?');
INSERT INTO questions (id, statement) VALUES(25, '¿En qué consiste la paginación bajo demanda?');


--Table "answers"
--First question
INSERT INTO answers VALUES(100, 'n*log(n)', 1, 1, 1);
INSERT INTO answers VALUES(101, 'n^2', 0, 2, 1);
INSERT INTO answers VALUES(102, 'n', 0, 3, 1);
INSERT INTO answers VALUES(103, 'n^3', 0, 4, 1);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Second question
INSERT INTO answers VALUES(104, 'n*log(n)', 0, 1, 2);
INSERT INTO answers VALUES(105, 'n^2', 1, 2, 2);
INSERT INTO answers VALUES(106, 'n^3', 0, 3, 2);
INSERT INTO answers VALUES(107, 'n', 0, 4, 2);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Third question
INSERT INTO answers VALUES(108, 'El orden de y theta', 0, 1, 3);
INSERT INTO answers VALUES(109, 'El orden de y omega', 0, 2, 3);
INSERT INTO answers VALUES(110, 'El orden de, omega y theta', 1, 3, 3);
INSERT INTO answers VALUES(111, 'Ninguna de las anteriores', 0, 4, 3);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Fourth question
INSERT INTO answers VALUES(112, 'Esquema Voraz', 0, 1, 4);
INSERT INTO answers VALUES(113, 'Divide y vencerás', 0, 2, 4);
INSERT INTO answers VALUES(114, 'Las tres restantes no son ciertas', 0, 3, 4);
INSERT INTO answers VALUES(115, 'Backtracking', 1, 4, 4);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Fifth question
INSERT INTO answers VALUES(116, 'Serie aritmética', 1, 1, 5);
INSERT INTO answers VALUES(117, 'Serie geométrica', 0, 2, 5);
INSERT INTO answers VALUES(118, 'Integrales', 0, 3, 5);
INSERT INTO answers VALUES(119, 'Derivadas', 0, 4, 5);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Sixth question
INSERT INTO answers VALUES(200, 'FIFO', 0, 1, 6);
INSERT INTO answers VALUES(201, 'LIFO', 1, 2, 6);
INSERT INTO answers VALUES(202, 'LILO', 0, 3, 6);
INSERT INTO answers VALUES(203, 'Ninguna de las anteriores', 0, 4, 6);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Seventh question
INSERT INTO answers VALUES(204, 'En nada, son iguales, pero con distinto nombre', 0, 1, 7);
INSERT INTO answers VALUES(205, 'Poseen un campo extra con un puntero', 0, 2, 7);
INSERT INTO answers VALUES(206, 'Poseen un campo extra que contiene una clave', 1, 3, 7);
INSERT INTO answers VALUES(207, 'Todas las anteriores son correctas', 0, 4, 7);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Eigth question
INSERT INTO answers VALUES(208, 'Un tipo de Conjunto Disjunto', 0, 1, 8);
INSERT INTO answers VALUES(209, 'Un tipo de Montículo Binario', 0, 2, 8);
INSERT INTO answers VALUES(210, 'Un tipo de Árbol Binario', 0, 3, 8);
INSERT INTO answers VALUES(211, 'Un grado cuyos vértices están unidos por las aristas de menor coste (de menos peso)', 1, 4, 8);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Ninth question
INSERT INTO answers VALUES(212, 'Listas enlazadas, árboles binarios y matrices', 1, 1, 9);
INSERT INTO answers VALUES(213, 'Listas enlazadas y árboles binarios', 0, 2, 9);
INSERT INTO answers VALUES(214, 'Únicamente mediantes matrices', 0, 3, 9);
INSERT INTO answers VALUES(215, 'listas enlazadas y matrices', 0, 4, 9);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Tenth question
INSERT INTO answers VALUES(216, 'Propiedas de recorrido y orden', 0, 1, 10);
INSERT INTO answers VALUES(217, 'Propiedas de estructura y orden', 1, 2, 10);
INSERT INTO answers VALUES(218, 'Propiedad de recorrido y estructura', 0, 3, 10);
INSERT INTO answers VALUES(219, 'Todas las demás son correctas', 0, 4, 10);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Eleventh question
INSERT INTO answers VALUES(300, 'Autómata Finito No Determinsita', 0, 1, 11);
INSERT INTO answers VALUES(301, 'Máquina de Turing', 0, 2, 11);
INSERT INTO answers VALUES(302, 'Autómata Finito Determinista', 1, 3, 11);
INSERT INTO answers VALUES(303, 'Conjunto Universal', 0, 4, 11);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Twelfth question
INSERT INTO answers VALUES(304, 'Ninguna de las siguientes', 0, 1, 12);
INSERT INTO answers VALUES(305, 'Forma de representar información basada en un conjunto finito de sólo símbolos', 0, 2, 12);
INSERT INTO answers VALUES(306, 'Forma de representar información basada en un conjunto infinito de signos o símbolos', 0, 3, 12);
INSERT INTO answers VALUES(307, 'Forma de representar información basada en un conjunto finito de signos o símbolos', 1, 4, 12);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Thirteenth question
INSERT INTO answers VALUES(308, 'Lenguaje descrito mediante un formalismo matemático', 1, 1, 13);
INSERT INTO answers VALUES(309, 'Lenguaje descrito mediante un formalismo físico', 0, 2, 13);
INSERT INTO answers VALUES(310, 'Lenguaje abstracto sobre cualquier cosa', 0, 3, 13);
INSERT INTO answers VALUES(311, 'Ninguna de las anteriores', 0, 4, 13);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Fourteenth question
INSERT INTO answers VALUES(312, 'Para compilar programas escritos en lenguaje de programación', 0, 1, 14);
INSERT INTO answers VALUES(313, 'En experimentos de pensamiento', 1, 2, 14);
INSERT INTO answers VALUES(314, 'Para poder escribir e-mails', 0, 3, 14);
INSERT INTO answers VALUES(315, 'Todas son ciertas', 0, 4, 14);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Fifteenth question
INSERT INTO answers VALUES(316, 'No puede simular ningún algoritmo. No está diseñada para eso', 0, 1, 15);
INSERT INTO answers VALUES(317, 'Puede simular casi cualquier algoritmo', 0, 2, 15);
INSERT INTO answers VALUES(318, 'Puede simular cualquier algoritmo', 1, 3, 15);
INSERT INTO answers VALUES(319, 'Todas las anteriores son ciertas', 0, 4, 15);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Sixteenth question
INSERT INTO answers VALUES(400, 'Debe de maximizar beneficios minimizando costes', 1, 1, 16);
INSERT INTO answers VALUES(401, 'Debe de minimizar beneficios maximizando costes', 0, 2, 16);
INSERT INTO answers VALUES(402, 'Debe de maximizar beneficios maximizando costes', 0, 3, 16);
INSERT INTO answers VALUES(403, 'Debe de minimizar beneficios minimizando costes', 0, 4, 16);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Seventeenth question
INSERT INTO answers VALUES(404, 'La empresa no forma parte de la Economía', 0, 1, 17);
INSERT INTO answers VALUES(405, 'La empresa está integrada enteramente dentro de la Economía', 1, 2, 17);
INSERT INTO answers VALUES(406, 'La empresa está integrada en la Economía únicamente a nivel de producción', 0, 3, 17);
INSERT INTO answers VALUES(407, 'Todas son ciertas', 0, 4, 17);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Eighteenth question
INSERT INTO answers VALUES(408, 'Generar bienes y servicios', 0, 1, 18);
INSERT INTO answers VALUES(409, 'Anticipar producto obtenido', 0, 2, 18);
INSERT INTO answers VALUES(410, 'Todas son verdaderas', 1, 3, 18);
INSERT INTO answers VALUES(411, 'Asumir riesgos', 0, 4, 18);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Nineteenth question
INSERT INTO answers VALUES(412, 'Independencia', 0, 1, 19);
INSERT INTO answers VALUES(413, 'Transformación', 0, 2, 19);
INSERT INTO answers VALUES(414, 'Entropía', 0, 3, 19);
INSERT INTO answers VALUES(415, 'Todas las anterires son correctas', 1, 4, 19);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Twentieth question
INSERT INTO answers VALUES(416, 'Determinar todo lo necesario para poner en marcha una empresa', 1, 1, 20);
INSERT INTO answers VALUES(417, 'Tener en cuenta las amortizaciones en el plan de viabilidad de una empresa', 0, 2, 20);
INSERT INTO answers VALUES(418, 'Calcular los costes de financiación', 0, 3, 20);
INSERT INTO answers VALUES(419, 'Todas son válidas', 0, 4, 20);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Twenty first question
INSERT INTO answers VALUES(500, 'Formatear los datos contenidos en un Disco Duro', 0, 1, 21);
INSERT INTO answers VALUES(501, 'Varias personas utilizando un mismo Sistema a la vez', 0, 2, 21);
INSERT INTO answers VALUES(502, 'Gestión de los mecanismos IPC del Sistema Operativo', 0, 3, 21);
INSERT INTO answers VALUES(503, 'Dar un buen servicio a los procesos de un Sistema', 1, 4, 21);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Twenty second question
INSERT INTO answers VALUES(504, 'Todas las restantes', 1, 1, 22);
INSERT INTO answers VALUES(505, 'Abstracción del hardware', 0, 2, 22);
INSERT INTO answers VALUES(506, 'Detección de errores así como su posible solución', 0, 3, 22);
INSERT INTO answers VALUES(507, 'Puesta en ejecución de programas', 0, 4, 22);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Twenty third question
INSERT INTO answers VALUES(508, 'Algo no inherente al Sistema Operativo', 0, 1, 23);
INSERT INTO answers VALUES(509, 'Una instancia de ejecución de un programa', 1, 2, 23);
INSERT INTO answers VALUES(510, 'Un componente del Disco Duro', 0, 3, 23);
INSERT INTO answers VALUES(511, 'Todas las anteriores son respuestas válidas', 0, 4, 23);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Twenty fourth question
INSERT INTO answers VALUES(512, 'Compartición de código y/o datos', 0, 1, 24);
INSERT INTO answers VALUES(513, 'Reubicación del código', 0, 2, 24);
INSERT INTO answers VALUES(514, 'Todas las demás', 1, 3, 24);
INSERT INTO answers VALUES(515, 'Protección de la memoria de cada proceso', 0, 4, 24);
-----------------------------------------------------------------------------------------------------------------------------------------------------------
--Twenty fifth question
INSERT INTO answers VALUES(516, 'En dividir el programa en páginas/marcos y cargar en memoria secundaria todos', 0, 1, 25);
INSERT INTO answers VALUES(517, 'En dividir el programa en páginas/marcos y cargar en memoria secundaria sólo los que se estén usando', 0, 2, 25);
INSERT INTO answers VALUES(518, 'En dividir el programa en páginas/marcos y cargar en memoria principal todos', 0, 3, 25);
INSERT INTO answers VALUES(519, 'En dividir el programa en páginas/marcos y cargar en memoria principal sólo los que se estén usando', 1, 4, 25);



--Table "exams"
INSERT INTO exams VALUES(1, 2007, 1, 1);
INSERT INTO exams VALUES(2, 2008, 2, 2);
INSERT INTO exams VALUES(3, 2005, 1, 3);
INSERT INTO exams VALUES(4, 2010, 1, 4);
INSERT INTO exams VALUES(5, 2015, 2, 5);



--Table "questions_themes"
INSERT INTO questions_themes VALUES(1, 1);
INSERT INTO questions_themes VALUES(2, 2);
INSERT INTO questions_themes VALUES(3, 3);
INSERT INTO questions_themes VALUES(4, 4);
INSERT INTO questions_themes VALUES(5, 5);
INSERT INTO questions_themes VALUES(6, 6);
INSERT INTO questions_themes VALUES(7, 7);
INSERT INTO questions_themes VALUES(8, 8);
INSERT INTO questions_themes VALUES(9, 9);
INSERT INTO questions_themes VALUES(10, 10);
INSERT INTO questions_themes VALUES(11, 11);
INSERT INTO questions_themes VALUES(12, 12);
INSERT INTO questions_themes VALUES(13, 13);
INSERT INTO questions_themes VALUES(14, 14);
INSERT INTO questions_themes VALUES(15, 15);
INSERT INTO questions_themes VALUES(16, 16);
INSERT INTO questions_themes VALUES(17, 17);
INSERT INTO questions_themes VALUES(18, 18);
INSERT INTO questions_themes VALUES(19, 19);
INSERT INTO questions_themes VALUES(20, 20);
INSERT INTO questions_themes VALUES(21, 21);
INSERT INTO questions_themes VALUES(22, 22);
INSERT INTO questions_themes VALUES(23, 23);
INSERT INTO questions_themes VALUES(24, 24);
INSERT INTO questions_themes VALUES(25, 25);



--Table "exams_questions"
INSERT INTO exams_questions VALUES(1, 1, 1, 0, 0);
INSERT INTO exams_questions VALUES(2, 2, 0, 1, 0);
INSERT INTO exams_questions VALUES(3, 3, 0, 0, 1);
INSERT INTO exams_questions VALUES(4, 4, 1, 0, 0);
INSERT INTO exams_questions VALUES(5, 1, 0, 1, 0);
INSERT INTO exams_questions VALUES(6, 2, 0, 0, 1);
INSERT INTO exams_questions VALUES(7, 3, 1, 0, 0);
INSERT INTO exams_questions VALUES(8, 4, 0, 1, 0);
INSERT INTO exams_questions VALUES(9, 5, 0, 0, 1);
INSERT INTO exams_questions VALUES(10, 1, 1, 0, 0);
INSERT INTO exams_questions VALUES(11, 2, 0, 1, 0);
INSERT INTO exams_questions VALUES(12, 3, 0, 0, 1);
INSERT INTO exams_questions VALUES(13, 4, 1, 0, 0);
INSERT INTO exams_questions VALUES(14, 5, 0, 1, 0);
INSERT INTO exams_questions VALUES(15, 1, 0, 0, 1);
INSERT INTO exams_questions VALUES(16, 2, 1, 0, 0);
INSERT INTO exams_questions VALUES(17, 3, 0, 1, 0);
INSERT INTO exams_questions VALUES(18, 4, 0, 0, 1);
INSERT INTO exams_questions VALUES(19, 5, 1, 0, 0);
INSERT INTO exams_questions VALUES(20, 1, 0, 1, 0);
INSERT INTO exams_questions VALUES(21, 2, 0, 0, 1);
INSERT INTO exams_questions VALUES(22, 3, 1, 0, 0);
INSERT INTO exams_questions VALUES(23, 4, 0, 1, 0);
INSERT INTO exams_questions VALUES(24, 5, 0, 0, 1);
INSERT INTO exams_questions VALUES(25, 1, 1, 0, 0);


--Table "subjects_teachers"
INSERT INTO subjects_teachers VALUES(1, 1);
INSERT INTO subjects_teachers VALUES(2, 2);
INSERT INTO subjects_teachers VALUES(3, 3);
INSERT INTO subjects_teachers VALUES(4, 4);
INSERT INTO subjects_teachers VALUES(5, 5);
INSERT INTO subjects_teachers VALUES(5, 6);
