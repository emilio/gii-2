
Table created.


Sequence dropped.


Sequence created.

CREATE TRIGGER BEFORE INSERT ON log
                      *
ERROR at line 1:
ORA-04071: missing BEFORE, AFTER or INSTEAD OF keyword 


CREATE TRIGGER update_log_on_author AFTER UPDATE ON autor
                                                    *
ERROR at line 1:
ORA-01031: insufficient privileges 



Commit complete.

