package javaapplication5;

/**
 *
 * @author Victor Barrueco <vbarruec@usal.es>
 */
public class Main {

    /**************************************************************************/
    /* METODO MAIN                                                            */
    /**************************************************************************/
    /**
     *
     * @param args
     */
    public static void main(String[] args) {
        String[] options = {"MOSTRAR", "AÑADIR", "ELIMINAR", "MODIFICAR", "ORDENAR"};
        
        Menu menu = new Menu(options);
        menu.run();
    }
    
}
