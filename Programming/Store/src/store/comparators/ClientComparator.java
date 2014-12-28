/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.comparators;

import java.util.Comparator;
import store.models.Client;

/**
 *
 * @author emilio
 */
public class ClientComparator implements Comparator<Client> {

    @Override
    public int compare(Client o1, Client o2) {
        return o1.getName().compareTo(o2.getName());
    }
    
}
