package javaapplication5;

import static coti.esdia.Esdia.readInt;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Victor Barrueco <vbarruec@usal.es>
 */
class Menu {
    File path;
    static int flag;
    static ArrayList<Reg> table=null;
    StringBuilder menu;
    String fileName="";
    int seed=50;
    int flagExists=0;
    int flagNoName=0;
    int flagSave=0;
        
    public Menu(String[] ordenes) {
        Scanner sc = new Scanner(System.in);
        
        /*Carga y crear archivo*/
        do{
            System.out.println("Introduce el nombre del archivo a cargar (ejemplo.bin): ");
            fileName=sc.nextLine();
            String temp = "/home/emilio/Escritorio/"+fileName;
            if(fileName.equals("")){
                flagNoName=1;
                table = new ArrayList<>();
                this.randomGen(seed);
                System.out.println("Generado registro aleatorio");
                break;
            }
            path = new File(temp);
            if( path.exists() ){
                table = this.load();
                if (table == null) {
                    table = new ArrayList<>();
                }
                break;
            }
            
            System.out.println("El archivo no existe");
        } while( true );
                
        menu = new StringBuilder("Seleccione una orden:\n");

        for (String s : ordenes) {
            menu.append(s).append("\n");
        }
        menu.append("SALIR \n\n");
    }
    
    /**************************************************************************/
    /* METODO PARA EL MENU                                                    */
    /**************************************************************************/
    void run() {
        String opt;
        Scanner sc = new Scanner(System.in);
        do {
            System.out.print(menu);

            opt = sc.nextLine();
            switch (opt.toUpperCase()) {
                case "MOSTRAR":
                    System.out.printf("%nMostrar el registro:%n%n");
                    this.show();
                    break;
                case "AÑADIR":
                    System.out.printf("%nAñadir un registro%n%n");
                    this.add();
                    break;
                case "ELIMINAR":
                    System.out.printf("%nEliminar un registro:%n%n");
                    this.remove();
                    break;
                case "MODIFICAR":
                    System.out.printf("%nModificar un registro:%n%n");
                    this.modify();
                    break;
                case "ORDENAR":
                    System.out.printf("%nOrdenar el registro:%n%n");
                    this.order();
                    Collections.sort(table);
                    break;              
                case "SALIR":
                    this.salir();
                    System.out.printf("%n");
                    break;
                default:
                    System.err.printf("%n%s - Opción incorrecta%n%n", opt);
                    break;
            }
        } while ( !opt.equalsIgnoreCase("salir") );
    }
    
    /**************************************************************************/
    /* METODO PARA VER LA LISTA                                               */
    /**************************************************************************/
    private void show() {
        System.out.println("|Fabricante  |Modelo      |Color       |Grosor      |Precio      |Num. Unids  |");
        System.out.println("+------------+------------+------------+------------+------------+------------+");
        for ( Reg r : table ){
            System.out.printf("|%-12s|%-12s|%-12s|%-12.2f|%-12.2f|%-12d|\n", r.getFabricante(), r.getModelo(), r.getColor(), r.getGrosor(), r.getPrecio(), r.getNumUnidades());
        }
        System.out.printf("\n");
    }
    
    
    
    /**************************************************************************/
    /* METODO PARA MOSTRAR LA LISTA CON INDICES                               */
    /**************************************************************************/
    private void showIndex() {
        for(int i=0 ; i<table.size(); i++){
            System.out.printf("| %4d|%-12s|%-12s|%-12s|%-12.2f|%-12.2f|%-12d|\n",i, table.get(i).getFabricante(),table.get(i).getModelo(),table.get(i).getColor(),table.get(i).getGrosor(),table.get(i).getPrecio(),table.get(i).getNumUnidades());
        }
    }
    
    /**************************************************************************/
    /* METODO PARA AÑADIR                                                     */
    /**************************************************************************/
    private void add() {
        Reg rotu = new Reg();
        rotu.add();
        table.add(rotu);
    }
    
    /**************************************************************************/
    /* METODO PARA ELIMINAR                                                   */
    /**************************************************************************/
    private void remove() {
        int reg;
        
        this.showIndex();
        reg = readInt("Escriba el índice"
                + " del registro borrado : ");
        table.remove(reg);
    }
    
    /**************************************************************************/
    /* METODO PARA MODIFICAR                                                  */
    /**************************************************************************/
    private void modify() {
        int reg;
                
        this.showIndex();
        reg = readInt("Escriba el número del rotulador a modificar");
        table.get(reg).add();

    }
    
    /**************************************************************************/
    /* METODO PARA ORDENAR (Hace falta volver al menu para mostrar)           */
    /**************************************************************************/
    private void order() {
        String opt;
        Scanner sc = new Scanner(System.in);
        
        do {
            System.out.println("Ordenar el registro por: ");
            System.out.printf("\t FABRICANTE\n");
            System.out.printf("\t MODELO\n");
            System.out.printf("\t COLOR\n");
            System.out.printf("\t GROSOR\n");
            System.out.printf("\t PRECIO\n");
            System.out.printf("\t NUMERO DE UNIDADES\n");
            System.out.printf("\t SALIR (Volver al menu anterior)\n");
            
            opt = sc.nextLine();
            switch (opt.toUpperCase()) {
                case "FABRICANTE":
                    System.out.printf("%nRotuladores ordenados por fabricante%n%n");
                    flag = 1;
                    break;
                case "MODELO":
                    System.out.printf("%nRotuladores ordenados por modelo%n%n");
                    flag = 2;
                    break;
                case "COLOR":
                    System.out.printf("%nRotuladores ordenados por color%n%n");
                    flag = 3;
                    break;
                case "GROSOR":
                    System.out.printf("%nRotuladores ordenados por grosor%n%n");
                    flag = 4;
                    break;
                case "PRECIO":
                    System.out.printf("%nRotuladores ordenados por precio%n%n");
                    flag = 5;
                    break;
                case "NUMERO DE UNIDADES":
                    System.out.printf("%nRotuladores ordenados por numero de unidades%n%n");
                    flag = 6;
                    break;
                default:
                    System.err.printf("%n%s - Opción incorrecta%n%n", opt);
                    break;
                case "SALIR":
                    System.out.printf("\n\n");
                    break;
            }
        } while (!opt.equalsIgnoreCase("salir"));
    }
    
    /**************************************************************************/
    /* METODO PARA GUARDAR Y SALIR                                            */
    /**************************************************************************/
    public void salir() {
    /*Guardar el archivo*/
        Scanner sc = new Scanner(System.in);

        if( ! fileName.equals("") ){
            this.save();
            System.out.println("Archivo guardado");
            return;
        }
        
        System.out.println("Guardar como (ejemplo.bin): ");
        fileName = sc.nextLine();
        
        if ( fileName.equals("") ) {
            System.out.println("Has salido sin guardar");
            return;
        }
        
        String temp = "/home/emilio/Escritorio/"+fileName;
        path = new File(temp);
        
        if ( ! path.exists() ) {
            try {
                path.createNewFile();
            } catch (IOException ex) {
                System.out.println("No se ha podido crear el archivo " + path);
                ex.printStackTrace();
                return;
            }
        }
        
        this.save();
    }
    
    /**************************************************************************/
    /* METODO PARA GUARDAR EL ARCHIVO                                         */
    /**************************************************************************/
    private void save() {
        try {
            FileOutputStream fos = new FileOutputStream(path);
            ObjectOutputStream oos = new ObjectOutputStream(fos);
            oos.writeObject(table);
            oos.close();
        } catch (Exception ex) {
            ex.printStackTrace();
            System.out.println("No fue posible guardar " + path);
            System.out.println(ex);
        }
    }
    
    /**************************************************************************/
    /* METODO PARA CARGAR EL ARCHIVO                                          */
    /**************************************************************************/
    private ArrayList<Reg> load() {
        ArrayList<Reg> temp;
        try {
            FileInputStream fis = new FileInputStream(path);
            ObjectInputStream ois = new ObjectInputStream(fis);
            temp = (ArrayList<Reg>) ois.readObject();
            ois.close();
        } catch (Exception ex) {
            ex.printStackTrace();
            System.out.println("Se ha creado el archivo " + path);
            temp = null;
        }
        return temp;
    }
    
    /**************************************************************************/
    /* METODO PARA CREAR UN REGISTRO ALEATORIO                                */
    /**************************************************************************/
    public void randomGen(int seed) {
        Random r;
        Reg reg;   
                
        String[] rndFabricante = { "Smoesa", "Alco", "Cophel", "Carioca", "Afa", "Esigal", "Innograf", "Zenit", "Ajace" };
        String[] rndModelo = { "Softline", "Blister", "Marcador", "Permanente", "Punta gord", "Caligrafia", "Corrector" };
        String[] rndColor = { "Negro", "Marron", "Rojo", "Magenta", "Rosa", "Naranja", "Amarillo", "Verde", "V. Oscuro", "Cyan", "Azul", "Violeta", "Morado" };
        float[] rndGrosor = { 2.3f, 1.2f, 0.8f, 2.5f, 1.5f, 0.5f, 1.3f };
        float[] rndPrecio = { 0.5f, 0.75f, 1.0f, 1.25f, 1.50f, 1.75f, 2.0f};
        int [] rndNumUnidades = {0, 16, 21, 33, 48, 51, 63, 7, 78, 14};
        
        for (int i=0; i < seed; i++) {
        
            r = new Random();
            reg = new Reg();
            
            reg.fabricante = rndFabricante[r.nextInt(rndFabricante.length)];
            reg.modelo = rndModelo[r.nextInt(rndModelo.length)];
            reg.color = rndColor[r.nextInt(rndColor.length)];
            reg.grosor = rndGrosor[r.nextInt(rndGrosor.length)];
            reg.precio = rndPrecio[r.nextInt(rndPrecio.length)];
            reg.numUnidades = rndNumUnidades[r.nextInt(rndNumUnidades.length)];
            
            table.add(reg);
        }        
    }
}
