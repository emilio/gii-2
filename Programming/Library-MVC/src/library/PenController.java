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
import java.util.Properties;
import java.util.Scanner;
import library.Pen;

class PenController implements IController {
    Scanner sc;
    private ArrayList<Pen> pens;
    private final int DEFAULT_PEN_COUNT = 50;
    private String fileName;

    /**
     * Loads a bunch of pens from `fileName`
     *
     * @return saved data
     */
    private ArrayList<Pen> loadPens() throws FileNotFoundException, IOException {
        ArrayList<Pen> p = null;
        FileInputStream fis = new FileInputStream(Utils.desktopFile(fileName));
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
    protected void savePens() throws FileNotFoundException, IOException {
        File file = new File(Utils.desktop(fileName));

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

    PenController(String fileName) {
	this.sc = new Scanner(System.in);
	this.fileName = fileName;
    }
    
    public void index() {
        PenIndexView view = new PenIndexView();
        Properties params = new Properties();
        view.render(pens, params);
    }

    public void exit() {
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

    public void delete(int id) {
        System.out.println("Write the item id");

        if ( id < 0 || id >= pens.size() ) {
            System.out.println("No pens removed");
        } else {
            pens.remove(id);
            System.out.println("Pen " + id + " removed successfully. Check the id's, they have changed");
        }
    }
}


