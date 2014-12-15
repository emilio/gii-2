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
abstract class Repository<Model> {
    public abstract ModelCollection<Model> find();
    public abstract ModelCollection<Model> find(Arguments args);
}
