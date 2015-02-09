/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvc;

/**
 *
 * @author emilio
 * @param <Model>
 */
public abstract class Repository<Model> {
    public abstract ModelCollection<Model> all();
    public abstract boolean add(Model m);
    public abstract boolean delete(Model m);
    public abstract boolean contains(Model m);
    public abstract boolean save();
    
    public boolean remove(Model m) {
        return this.delete(m);
    }
}
