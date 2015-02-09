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
public class Refill extends Model {
    private final String id;
    private String name;
    private float price;
    private int stock;
    
    public Refill(String id, String name, float price) {
        this.id = id;
        this.name = name; 
        this.price = price;
        this.stock = 0;
    }
    
    public Refill(String id, String name, float price, int stock) {
        this.id = id;
        this.name = name; 
        this.price = price;
        this.stock = stock;
    }
    
    public String getId() {
        return id;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public float getPrice() {
        return price;
    }
    
    public void setPrice(float price) {
        this.price = price;
    }
    
    public int getStock() {
        return stock;
    }
    
    public void setStock(int stock) {
        this.stock = stock;
    }

    public void addStock(int s) {
        this.stock += s;
    }
    
    @Override
    public String toString() {
        return "[" + id + ":" + stock + "] " + name + "\t" + price;
    }

}
