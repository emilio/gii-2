package store;

import store.repositories.ClientsRepository;
import store.repositories.ProvidersRepository;
import store.repositories.PurchasesRepository;
import store.repositories.RefillsRepository;
import store.repositories.SalesRepository;
import mvc.Application;
import mvc.InvalidActionException;
import mvc.InvalidAppPathException;
import mvc.InvalidControllerException;

/**
 * NOTAS ACERCA DE LA PRÁCTICA:
 *
 * El sistema empleado está lejos del MVC de Coti, este se asemeja
 * al modelo vista controlador de una aplicación Rails:
 *  - El modelo almacena los datos, pero nunca la lógica de la aplicación
 *    (véase la clase mvc.Model y las clases en store.models)
 *  - El controlador lleva toda la lógica, y ha hay un controlador por modelo,
 *    más un controlador llamado `MainController` para el menú principal
 *  - Las vistas contienen la interfaz mostrada al usuario (con la excepción de
 *    los mensajes de error, etc que se muestran en el controlador para no sobre-
 *    complicar la aplicación)
 * 
 * Dados los ficheros iniciales clientes.bin, proveedores.bin y recambios.encol
 * se han proporcionado formas de importación, pero no se leerán automáticamente
 * al comienzo de la aplicación. No obstante sí persistirán en formato binario
 * una vez importado, el por defecto dado nuestro modo de almacenamiento 
 * (ver mvc.Repository y mvc.FileBasedRepository).
 * 
 * La exportación e importación de compras y ventas se realiza por defecto en tal formato,
 * y se podrá comprobar que persisten a la ejecución una vez creadas.
 * 
 * La sincronización entre compras, ventas y stock es inmediata, reduciéndose el stock
 * al guardar una venta, y aumentándose al guardar una compra.
 * 
 * Soy muy excéptico con la edición de ventas ya que es más simple anular una y crear una nueva,
 * la edición anade un nivel extra de complejidad que en mi opinión no es necesario.
 * No obstante está implementada.
 * 
 * Por último me gustaría comentar lo desquiciante que es crear una base de datos sin un sistema 
 * de bases de datos...
 * 
 * SOBRE LAS FUNCIONALIDADES:
 *  - Compras (Purchases):
 *     * Listado de proveedores por identificador (Show providers)
 *     * Efectuar compra (Create purchase)
 *     * Anular compra (Delete purchase)
 *     * Informes de compras:
 *         + Listado de compras ordenado por identificador del proveedor (Show purchases by provider id)
 *         + Listado ordenado por valor total de la compra (Show purchases by value)
 *  - Ventas (Sales):
 *     * Listado de clientes por nombre (Show clients)
 *     * Efectuar venta (Create sale)
 *     * Anular venta (Delete sale)
 *     * Modificar venta (Edit sale)
 *     * Informes de Ventas:
 *         + Listado de ventas ordenado por DNI del cliente (Show sales by client id)
 *         + Listado de ventas ordenado por valor total de la venta (Show sales by value)
 *  - Importación y exportación:
 *     Como he comentado antes las compras y las ventas persisten entre ejecuciones, por
 *     lo que no he considerado necesario la exportación e importación. Además la importación
 *     en un caso real podría causar que el stock de productos en la aplicación quedara corrupto 
 *     (al final en la realidad todas las compras y las ventas deberían estar declaradas,
 *     por lo que por la seguridad de la empresa haría meterlas a mano, yo no permitiría la
 *     importación).
 */

/**
 *
 * @author emilio
 */
public class Store {
    /**
     * @param args the command line arguments
     * @throws mvc.InvalidActionException
     * @throws mvc.InvalidControllerException
     * @throws mvc.InvalidAppPathException
     */
    public static void main(String[] args) throws InvalidActionException, InvalidControllerException, InvalidAppPathException  {
        Application.addRepository("clients", new ClientsRepository());
        Application.addRepository("providers", new ProvidersRepository());
        Application.addRepository("refills", new RefillsRepository());
        Application.addRepository("sales", new SalesRepository());
        Application.addRepository("purchases", new PurchasesRepository());
        Application app = new Application("store");
        
        app.execute("main.index", null);
    }
}
