/** 1- Crear un índice en la tabla LECTOR para el campo PROVINCIA */
CREATE INDEX test ON univ.lector(univ.provincia);

/** 2 */
CREATE VIEW active AS SELECT isbn, cod_lector FROM univ.prestamo WHERE fecha_dev IS NULL;

/** 3 */
CREATE VIEW loaned_books AS SELECT isbn, titulo FROM univ.libro WHERE isbn IN (SELECT DISTINCT isbn FROM univ.prestamo WHERE fecha_dev IS NULL);

/** 3 */
SELECT * FROM loaned_books;

/** 4 */
CREATE VIEW sucursal_3 AS SELECT l.titulo, l.isbn, d.num_ejemplares, d.num_disponibles FROM univ.dispone d, univ.libro l WHERE cod_suc = 3 AND d.isbn = l.isbn;

