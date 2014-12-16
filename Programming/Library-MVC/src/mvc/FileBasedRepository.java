/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvc;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

/**
 *
 * @author emilio
 * @param <Model>
 */
public abstract class FileBasedRepository<Model> extends Repository<Model> {
    private ModelCollection<Model> all;
    private String fileName = null;
        
    @Override
    public ModelCollection<Model> all() {
        return all;
    }
    
    @Override
    public boolean add(Model m) {
        all.add(m);
        return true;
    }
    
    @Override
    public boolean delete(Model toDelete) {
        return all.remove(toDelete);
    }

    
    
    public boolean save() {
        try {
            File f = new File(getFileName());
            FileOutputStream fos;
            ObjectOutputStream oos;
            
            if ( ! f.exists() )
                f.createNewFile();
            
            fos = new FileOutputStream(f);
            oos = new ObjectOutputStream(fos);
            
            oos.writeObject(all);
            
            oos.close();
            fos.close();
            
            return true;
        } catch (IOException ex) {
            ex.printStackTrace();
            return false;
        }
    }
    
    public boolean load() {
        try {
            File f = new File(getFileName());
            FileInputStream fis;
            ObjectInputStream ois;
            
            if ( ! f.exists() )
                f.createNewFile();
            
            fis = new FileInputStream(f);
            ois = new ObjectInputStream(fis);
            
            all = (ModelCollection<Model>) ois.readObject();
            
            ois.close();
            fis.close();
            
            return true;
        } catch (IOException | ClassNotFoundException ex) {
            ex.printStackTrace();
            all = new ModelCollection<>();
            return false;
        }
    }
    
    public String getFileName() {
        if ( fileName == null )
            fileName = this.getClass().getName().toLowerCase() + ".bin";
        
        return fileName;
    }
}
