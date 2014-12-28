Readme - PRACTICA PIII 2015
===========================

Los archivos clientes.bin y proveedores.bin poseen la estructura habitual de archivos binarios heterogéneos, como se puede ver en el proyecto FlujosDeDatos, en el cual se escribían y leían datos enteros, alfanuméricos y de doble precision. En este caso se almacenan los atributos de la clase Cliente o Proveedor, por el orden que se indica a continuación

Los Clientes poseen estos atributos: 

DNI
direccion
nombre

todos ellos de tipo String.

Los Proveedores poseen estos atributos:

direccion
NIF
nombre

todos ellos de tipo String

Los dos archivos contienen en primer lugar un número entero que indica el número de elementos, y a continuación cadenas distintas en formato binario para cada trío de atributos. La lectura, por tanto, obtiene primero el número de elementos y después va cargando atributos y añadiendo elementos a las listas de Clientes o Proveedores respectivamente. Es conveniente, por tanto, disponer de constructores con tres argumentos en las clases Cliente y Proveedor.

El archivo ZIP contiene un archivo encolumnado llamado recambios.encol con los datos de un grupo de Recambios que pueden utilizarse como ejemplo. Cada Recambio posee los campos

nombre
identificador
precio
número de unidades

donde el nombre y el identificador son de alfanuméricos, el precio es float y el número de unidades es entero. Se puede leer este archivo a principio de programa, disponiendo así de un grupo fijo de Recambios.

