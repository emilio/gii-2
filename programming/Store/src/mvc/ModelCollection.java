/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvc;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;

/**
 *
 * @author emilio
 * @param <Model>
 */
public class ModelCollection<Model> extends ArrayList<Model> {
    /**
     * Find elements by a field
     * @param field
     * @param value
     * @return 
     */
    public ModelCollection<Model> findBy(String field, Comparable value) {
        ModelCollection<Model> ret = new ModelCollection<>();
        String getterName = "get" + field.substring(0, 1).toUpperCase() + field.substring(1);
        
        if ( this.size() == 0 ) {
            return ret;
        }
        Method getter;
        try {
            getter = this.get(0).getClass().getMethod(getterName);
            
            for ( Model m : this )
                if ( value.compareTo(getter.invoke(m)) == 0 )
                    ret.add(m);

        } catch ( NoSuchMethodException | 
                  SecurityException | 
                  IllegalAccessException | IllegalArgumentException |
                  InvocationTargetException
                ex) {}
        
        return ret;
    }
    

    /**
     * Get first element by some field
     * @param field
     * @param value
     * @return 
     */
    public Model findFirstBy(String field, Comparable value) {
        String getterName = "get" + field.substring(0, 1).toUpperCase() + field.substring(1);
        
        if ( this.size() == 0 ) {
            return null;
        }

        Method getter;
        
        try {
            getter = this.get(0).getClass().getMethod(getterName);
            
            for ( Model m : this )
                if ( value.compareTo(getter.invoke(m)) == 0 )
                    return m;

        } catch ( NoSuchMethodException | 
                  SecurityException | 
                  IllegalAccessException | IllegalArgumentException |
                  InvocationTargetException
                ex) {}
        
        return null;
    }
    
    /**
     * This method should be overriden, probably by
     * an interaction with some type of db connection
     * @param id
     * @return 
     */
    public Model getElement(int id) {
        return this.get(id);
    }
}
