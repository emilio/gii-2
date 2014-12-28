/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.models;

import mvc.Model;

/**
 *
 * @author emilio
 */
public final class Provider extends Model {
    private final String nif;
    private String name;
    private String direction;
    
    public Provider(String nif, String name, String direction) {
        this.nif = nif;
        this.name = name;
        this.direction = direction;
    }

    @Override
    public Object getId() {
        return nif;
    }
    
    public final String getNif() {
        return nif;
    }
    
    public final String getName() {
        return name;
    }
    
    public final void setName(String name) {
        this.name = name;
    }
    
    public final String getDirection() {
        return direction;
    }
    
    public final void setDirection(String direction) {
        this.direction = direction;
    }
    
    @Override
    public String toString() {
        return nif + "\t" + name + "\t" + direction;
    }
}
