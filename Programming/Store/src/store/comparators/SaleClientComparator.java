/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.comparators;

import java.util.Comparator;
import store.models.Sale;

/**
 *
 * @author emilio
 */
public class SaleClientComparator implements Comparator<Sale> {
    @Override
    public int compare(Sale o1, Sale o2) {
        return o1.getClientId().compareTo(o2.getClientId());
    }
    
}
