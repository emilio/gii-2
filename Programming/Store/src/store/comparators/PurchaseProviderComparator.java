/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.comparators;

import java.util.Comparator;
import store.models.Purchase;

/**
 *
 * @author emilio
 */
public class PurchaseProviderComparator implements Comparator<Purchase> {

    @Override
    public int compare(Purchase o1, Purchase o2) {
        return o1.getProviderId().compareTo(o2.getProviderId());
    }
    
}
