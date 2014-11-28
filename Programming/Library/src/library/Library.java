/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Scanner;

/**
 *
 * @author emilio
 */
public class Library {
    private static ArrayList<Pen> pens;
    private static final int DEFAULT_PEN_COUNT = 50;
    private static String fileName;
    
    /**
     * Loads a bunch of pens from `fileName`
     *
     * @return saved data
     */
    private static ArrayList<Pen> loadPens() throws FileNotFoundException, IOException {
        ArrayList<Pen> p = null;
        // File f = new File(fileName);
        FileInputStream fis = new FileInputStream(inDesktop(fileName));
        ObjectInputStream ois = new ObjectInputStream(fis);
        
        try {
            p = (ArrayList<Pen>) ois.readObject();
        } catch (ClassNotFoundException ex) {}
        
        if ( p == null ) {
            throw new IOException("Invalid file contents");
        }
        
        return p;
    }
    
    /**
     * Save pens to `fileName`, create it if doesn't exists
     * 
     * @throws java.io.IOException
     * @throws java.io.FileNotFoundException
     */
    protected static void savePens() throws FileNotFoundException, IOException {
        File file = new File(inDesktop(fileName));
        
        if ( ! file.exists() ) {
            try {
                file.createNewFile();
            } catch( IOException ex ) {
                System.out.println("Couldn't create " + file.toString());
                ex.printStackTrace();
                return;
            }
        }
        FileOutputStream fos = new FileOutputStream(file);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        
        oos.writeObject(pens);
    }
    
    /**
     * We get the pens via a binary file
     * Or generate them randomly
     */
    private static void getPens() {
        Scanner sc = new Scanner(System.in);
        System.out.println("Introduce un nombre de fichero binario para cargar la colección (dejar en blanco para generar aleatoriamente): ");
        fileName = sc.nextLine();
        if ( fileName.equals("") ) {
            pens = new ArrayList<>();
            for ( int i = 0; i < DEFAULT_PEN_COUNT; i++ ) {
                pens.add(new Pen());
            }
            return;
        }
        
        try {
            pens = loadPens();
        } catch(FileNotFoundException ex) {
            System.out.println("File not found");
            getPens();
        } catch (IOException ex) {
            System.out.println("Unknown IO error");
            ex.printStackTrace();
            getPens();
        }
    }

    
    /**
     * Show the currently saved pens
     */
    public static void showPens() {
        displayPens(pens);
    }

    /**
     * Show pens as a table
     * @param pens
     */
    private static void displayPens(ArrayList<Pen> penList) {
        String headFormat = "| %5s | %-20s | %-5s | %-20s | %7s | %7s | %-5s |\n";
        String rowFormat = "| %5d | %-20s | %-5s | %-20s | %5.2f | %5.2f | %-5d |\n";
        String separator = "+-------+----------------------+-------+---------------------+--------+-------+";
        System.out.println(separator);
        System.out.printf(headFormat, "Id", "Manufacturer", "Model", "Color", "Price", "Thickness", "Stock");
        
        for ( Pen p: penList ){
            System.out.println(separator);            
            System.out.printf(rowFormat, pens.indexOf(p), p.getManufacturer(), p.getModel(), p.getColor(), p.getPrice(), p.getThickness(), p.getStock());
        }
        System.out.println(separator);
    }
    
    /**
     * Show pens ordered by any of their fields
     * @see PenColorComparator
     * @see PenManufacturerComparator
     * ...etc
     */
    public static void orderPens() {
        Scanner sc = new Scanner(System.in);
        System.out.println("Give me the field name: ");
        String field = sc.nextLine().trim().toLowerCase();
        Comparator c;
        ArrayList<Pen> sorted;
        try {
            c = Pen.getComparator(field);
        } catch (ClassNotFoundException ex) {
            ex.printStackTrace();
            return;
        } catch (/* ClassNotFoundException | */ InstantiationException | IllegalAccessException ex) {
            System.out.println("Could't create a comparator for field " + field);
            return;
        }
        sorted = (ArrayList<Pen>) pens.clone();
        Collections.sort(sorted, c);
        displayPens(sorted);
    }
    
    /**
     * Get a new pen from System.in
     * 
     * @return the new pen
     */
    private static Pen getPen() {
        Scanner sc = new Scanner(System.in);
        
        System.out.println("Manufacturer: ");
        String manufacturer = sc.nextLine();
        
        System.out.println("Model: ");
        String model = sc.nextLine();
        
        System.out.println("Color: ");
        String color = sc.nextLine();
        
        // No validations, too lazy
        System.out.println("Price: ");
        float price = sc.nextFloat();
        
        System.out.println("Thickness: ");
        float thickness = sc.nextFloat();
        
        System.out.println("Stock: ");
        int stock = sc.nextInt();
        
        return new Pen(manufacturer, model, color, price, thickness, stock);
    }
    
    /**
     * Add pen at the end of the pens ArrayList
     */
    public static void addPen() {
        pens.add(getPen());
    }
    
    /**
     * Edit a pen asking for id
     * 
     * @note Just edits stock and price, this is a design decission, since all other properties are inmutable
     */
    public static void editPen() {
        Scanner sc = new Scanner(System.in);
        System.out.println("Write the id: ");
        int id = sc.nextInt();
        Pen pen = pens.get(id);
        
        if ( pen == null ) {
            System.out.println("Unexisting pen");
            return;
        }
        
        int stock = -1;
        while ( stock < 0 ) {
            System.out.println("New stock (old = " + pen.getStock() +"):");
            stock = sc.nextInt();
        }
        
        float price = -1;
        
        while ( price < 0 ) {
            System.out.println("New price (old = " + pen.getPrice() +"):");
            price = sc.nextFloat();
        }
        
        pen.setStock(stock);
        pen.setPrice(price);
    }
    
    private static void alterPen(boolean add) {
    
    }
    
    public static void removePen() {
        Scanner sc = new Scanner(System.in);
        System.out.println("Write the item id");
        int id = sc.nextInt();
        if ( id < 0 || id >= pens.size() ) {
            System.out.println("No pens removed");
        } else {
            pens.remove(id);
            System.out.println("Pen " + id + " removed successfully. Check the id's, they have changed");
        }
    }
    
    public static void exit() {
        Scanner sc = new Scanner(System.in);
        if ( fileName.equals("") ) {
            System.out.println("Select file name to save");
            fileName = sc.nextLine();
        }
        
        if ( ! fileName.equals("") ) {
            try {
                savePens();
            } catch (FileNotFoundException ex) {
                System.out.println("File not found");
                fileName = "";
                exit();
            } catch ( IOException ex ) {
                System.out.println("Unknown IO error");
                ex.printStackTrace();
            }
        }
        
        System.exit(0);
    }

    /**
     * @param args the command line arguments
     * @throws java.lang.NoSuchMethodException
     * @throws java.lang.IllegalAccessException
     * @throws java.lang.reflect.InvocationTargetException
     */
    public static void main(String[] args) throws NoSuchMethodException, IllegalAccessException, IllegalArgumentException, InvocationTargetException {
        Menu menu = new Menu(Library.class, "Library");
        menu.addEntry("showPens", "Show");
        menu.addEntry("addPen", "Add");
        menu.addEntry("removePen", "Remove");
        menu.addEntry("editPen", "Edit");
        menu.addEntry("orderPens", "Order");
        menu.addEntry("exit", "Exit");
        
        
        getPens();
        
        while(true) {
            menu.render().getResponse().execute();
        }
    }

    private static String inDesktop(String fileName) {
        // return System.getProperty("user.home") + "/Desktop/" + fileName;
        return System.getProperty("user.home") + "/Escritorio/" + fileName; // I'm
    }
}
