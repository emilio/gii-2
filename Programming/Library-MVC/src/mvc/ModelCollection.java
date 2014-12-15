/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvc;

import java.util.ArrayList;

/**
 *
 * @author emilio
 */
class ModelCollection<Model> extends ArrayList<Model> {
    /**
     * This method should be overriden, probably by
     * an interaction with some type of db connection
     */
    public Model getElement(int id) {
        return this.get(id);
    }
    
    /**
     * This method should be overriden
     */
    public boolean save() {
        return true;
    }
}
