/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.models;

import mvc.Application;
import mvc.Model;

/**
 *
 * @author emilio
 */
public class Sale extends Model {
    private final int id;
    private String clientId;
    private String refillId;
    private int count;
    private transient Client client = null;
    private transient Refill refill = null;
    
    public Sale(int id, String clientId, String refillId, int count) {
        this.clientId = clientId;
        this.refillId = refillId;
        this.count = count;
        this.id = id;
    }
    
    public Sale(String clientId, String refillId, int count) {
        this.clientId = clientId;
        this.refillId = refillId;
        this.count = count;
        this.id = getNewId();
    }
    
    public Sale(String clientId, String refillId) {
        this.clientId = clientId;
        this.refillId = refillId;
        this.count = 1;
        this.id = getNewId();
    }
    
    @Override
    public Integer getId() {
        return id;
    }
    
    public String getClientId() {
        return clientId;
    }

    public void setClientId(String id) {
        this.clientId = id;
        this.client = null;
    }

    public Client getClient() {
        if ( client == null )
            client = (Client) Application.getRepository("clients").all().findFirstBy("id", clientId);
        return client;
    }
    
    public String getRefillId() {
        return refillId;
    }
    
    public void setRefillId(String refillId) {
        this.refillId = refillId;
        this.refill = null;
    }
    
    public Refill getRefill() {
        if ( refill == null )
            refill = (Refill) Application.getRepository("refills").all().findFirstBy("id", refillId);
        return refill;
    }
    
    public float getValue() {
        if ( getRefill() == null )
            return 0;
        return getRefill().getPrice() * count;
    }
    
    public int getCount() {
        return count;
    }
    
    public void setCount(int count) {
        this.count = count;
    }
    
    @Override
    public boolean valid() {
        return getClient() != null && getRefill() != null && getRefill().getStock() >= count;
    }
    
    public String getErrorMessage() {
        if ( getClient() == null )
            return "Invalid client";
        if ( getRefill() == null )
            return "Invalid refill";
        if ( getRefill().getStock() < count )
            return "Not enough stock";
        return "";
    }
    
    @Override
    public String toString() {
        return id + "\t" + refillId + "x" + count + "\t" + clientId + "\t" + getValue() + " (" + getClient() + ")";
    }
}
