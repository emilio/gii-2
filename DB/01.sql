ALTER SESSION SET CURRENT_SCHEMA = univ;

/*
 * 1 - Toda la información de autores (a), sucursales (b), lectores (c), libros (d) y editoriales (e)
 */
/* a) */
SELECT autor.*, nacionalidad.nombre AS nacionalidad
	FROM autor, nacionalidad
	WHERE nacionalidad.codigo = autor.cod_nacion;
/* b) */
SELECT * FROM sucursal;
/* c) */
SELECT * FROM lector;
/* d) */
SELECT libro.*, editorial.nombre AS editorial
	FROM libro, editorial 
	WHERE editorial.codigo = libro.cod_editorial;
/* e) */
SELECT * FROM editorial;


/*
 * 2 - Obtener la fecha de nacimiento de cada uno de los lectores orde nados del más joven al mayor de ellos
 */
SELECT codigo, fecha_nac FROM lector ORDER BY fecha_nac DESC;


/*
 * 3 - Obtener el ISBN de los libros que están prestados indicando la sucursal y la fecha en la que se realizó el préstamo.
 *     Ordenar la salida por sucursal y fecha de préstamo. 
 */
SELECT isbn, cod_suc, fecha_ini FROM prestamo WHERE fecha_dev IS NULL ORDER BY cod_suc, fecha_ini;

/*
 * 4 - Obtener el código y nombre de las editoriales. Incluir entre ambos campos el literal NOMBRE.
 */


/*
 * 5 - Obtener primer apellido, segundo apellido y nombre de todos los lectores ordenados por primer y segundo apellido
 */
SELECT ape_1, ape_2, nombre FROM lector ORDER BY ape_1, ape_2;

/*
 * 6 - Obtener un listado de los libros ordenado de menor a mayor antigüedad. 
 */
SELECT * FROM libro ORDER BY ano_edicion DESC;

/*
 * 7 - Obtener las distintas poblaciones en las que están domiciliados los lectores,
 * dando el nombre de la población y la provincia de cada una de ellas.
 * El listado deberá obtenerse ordenado por nombre de provincia y población.
 * Comparar el resultado que se hubiera obtenido, de no utilizar DISTINCT. 
 */
SELECT provincia, COUNT(provincia) AS num_lectores FROM lector GROUP BY provincia ORDER BY num_lectores DESC;

/*
 * 8 - Obtener el domicilio del lector cuyo segundo apellido sea BENITO.
 */
SELECT direccion, poblacion, provincia FROM lector WHERE UPPER(ape_2) = 'BENITO';

/*
 * 9 - Obtener el nombre completo de los autores con nacionalidad española.
 *     Busque previamente el código correspondiente en la tabla nacionalidad
 */
SELECT nombre || ' ' || apellido AS nombre FROM autor WHERE cod_nacion = (SELECT cod_nacion FROM nacionalidad WHERE nombre = 'ESPANA');

/*
 * 10 - Obtener la dirección postal completa de la sucursal cuyo código es 12 
 */
SELECT direccion || ', ' || provincia || ', ' || poblacion AS direccion_postal FROM sucursal WHERE codigo = 12;

/*
 * 11 - De los diferentes libros de los que disponen las sucursales
 * indicar aquellas sucursales que tienen más de 3 ejemplares de un mismo libro.
 * Indique el código de la sucursal, el ISBN del libro y el número de ejemplares con el requisito requerido
 * (3 o más de 3 ejemplares). 
 */
SELECT cod_suc, isbn, num_ejemplares FROM dispone WHERE num_ejemplares >= 3;

/*
 * 15 - Obtener un listado de los lectores que a lo largo de este año superan los 23 años. 
 */
SELECT * FROM lector WHERE (to_char(sysdate, 'yyyy') - to_char(fecha_nac, 'yyyy')) > 23 ORDER BY fecha_nac;
/* La anterior funciona pero esta es más correcta: */
SELECT * FROM lector WHERE (to_number(to_char(sysdate, 'yyyy')) - to_number(to_char(fecha_nac, 'yyyy'))) > 23 ORDER BY fecha_nac;


/*
 * 26 - Seleccionar los lectores que han tengan edades comprendidas entre los 26 y los 36 años.
 * 	Ordenar de mayor a menor edad. 
 */
/* Default oracle date arithmetics is day */
SELECT ape_1 || ' ' || ape_2 || ' ' || nombre AS nombre, fecha_nac, floor((sysdate - fecha_nac) / 365) AS edad FROM lector WHERE floor((sysdate - fecha_nac) / 365) BETWEEN 26 AND 36 ORDER BY floor((sysdate - fecha_nac) / 365) DESC;
