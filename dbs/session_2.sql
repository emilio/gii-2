UPDATE univ.dispone SET num_ejemplares = num_ejemplares + 3, num_disponibles = num_disponibles + 3 WHERE isbn = '5025496' AND cod_suc = 9;

/* Incrementar en dos unidades disponibles por sucursal el libro del que más préstamos se realizan. */
UPDATE univ.dispone d
	SET num_ejemplares = num_ejemplares + 2, num_disponibles = num_disponibles + 2
	WHERE d.isbn = (
		SELECT isbn FROM (SELECT p.isbn FROM univ.prestamo p GROUP BY p.isbn ORDER BY COUNT(p.isbn) DESC) WHERE ROWNUM <= 1);

/* Realizar un incremento en 1 ejemplar en todas las sucursales de aquellos libros para
los se han contabilizado más de 4 préstamos. */
UPDATE univ.dispone d
	SET num_ejemplares = num_ejemplares + 1, num_disponibles = num_disponibles + 1
	WHERE d.isbn IN (
		SELECT p.isbn FROM univ.prestamo p WHERE COUNT(p.isbn) > 4 GROUP BY p.isbn);
