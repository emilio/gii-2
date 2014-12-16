/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import mvc.Model;

/**
 *
 * @author emilio
 */
class Client extends Model {
    private final String dni;
    private String name;
    private String direction;
    
    public Client(String dni, String name, String direction) {
        this.dni = dni;
        this.name = name;
        this.direction = direction;
    }
    
    public String getDni() {
        return dni;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public String getDirection() {
        return direction;
    }
    
    public void setDirection(String direction) {
        this.direction = direction;
    }    
}
