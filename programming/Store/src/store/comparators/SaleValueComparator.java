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
public class SaleValueComparator implements Comparator<Sale> {

    @Override
    public int compare(Sale o1, Sale o2) {
        Float f1 = o1.getValue();
        return f1.compareTo(o2.getValue());
    }
    
}
