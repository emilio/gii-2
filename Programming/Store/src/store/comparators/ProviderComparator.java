/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.comparators;

import java.util.Comparator;
import store.models.Provider;

/**
 *
 * @author emilio
 */
public class ProviderComparator implements Comparator <Provider> {

    @Override
    public int compare(Provider o1, Provider o2) {
        return o1.getNif().compareTo(o2.getNif());
    }
    
}
