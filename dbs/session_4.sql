/** C'mon... The line below doesn't work for olivo*/
/** DROP TABLE IF EXISTS log; */

CREATE TABLE log (
	id INTEGER NOT NULL,
	action VARCHAR2(255) NOT NULL,
	object_type VARCHAR2(255) NOT NULL,
	object_id INTEGER NOT NULL,
	table_fields CLOB NOT NULL,
	PRIMARY KEY(id),
	CONSTRAINT log_o_type_id UNIQUE (object_type, object_id)
);
DROP SEQUENCE log_id_seq;
CREATE SEQUENCE log_id_seq;

CREATE TRIGGER update_log_on_author AFTER UPDATE ON autor
FOR EACH ROW
	SELECT log_id_sql.NEXTVAL INTO :new.id FROM dual;
BEGIN
	INSERT INTO log (`action`, `object_type`, `object_id`, `table_fields`) VALUES (
		'UPDATE', 'author', :new.codigo, 'nombre:' || :new.nombre || '||apellido:' || :new.apellido || '||ano_nac:' || TO_CHAR(:new.ano_nac) || '||ano_fall:' || TO_CHAR(:new.ano_fall) || '||cod_nacion:' || TO_CHAR(:new.cod_nacion));
END;
/
