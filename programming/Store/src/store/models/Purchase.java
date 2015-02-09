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
public final class Purchase extends Model {
    private final int id;
    private String refillId;
    private String providerId;
    private int count;
    
    private transient Provider provider = null;
    private transient Refill refill = null;

    public Purchase(int id, String providerId, String refillId, int count) {
        this.id = id;
        this.refillId = refillId;
        this.providerId = providerId;
        this.count = count;
    }
    
    public Purchase(String providerId, String refillId, int count) {
        this.id = getNewId();
        this.refillId = refillId;
        this.providerId = providerId;
        this.count = count;
    }
    
    public Purchase(String providerId, String refillId) {
        this.id = getNewId();
        this.refillId = refillId;
        this.providerId = providerId;
        this.count = 1;
    }
    
    @Override
    public Integer getId() {
        return id;
    }

    public String getRefillId() {
        return refillId;
    }
    
    public Refill getRefill() {
        if ( refill == null )
            refill = (Refill) Application.getRepository("refills").all().findFirstBy("id", refillId);
        return refill;
    }
    
    public String getProviderId() {
        return providerId;
    }
    
    public Provider getProvider() {
        if ( provider == null )
            provider = (Provider) Application.getRepository("providers").all().findFirstBy("id", providerId);
        return provider;
    }
    
    public int getCount() {
        return count;
    }

    public void setRefillId(String refillId) {
        this.refillId = refillId;
        this.refill = null;
    }

    public void setProviderId(String providerId) {
        this.providerId = providerId;
        this.provider = null;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public float getValue() {
        if ( getRefill() == null )
            return 0;
        return getRefill().getPrice() * count;
    }
    
    @Override
    public boolean valid() {
        return getRefill() != null && getProvider() != null;
    }
    
    public String getErrorMessage() {
        if ( getRefill() == null )
            return "Invalid refill";
        if ( getProvider() == null )
            return "Invalid provider";
        return "";
    }
        
    @Override
    public String toString() {
        return id + "\t" + refillId + "x" + count + "\t" + providerId + "\t" + getValue() + " (" + getProvider() + ")";
    }
}
