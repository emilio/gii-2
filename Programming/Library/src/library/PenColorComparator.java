/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import java.util.Comparator;

/**
 *
 * @author emilio
 */
public class PenColorComparator implements Comparator <Pen> {

    @Override
    public int compare(Pen o1, Pen o2) {
        return o1.getColor().compareTo(o2.getColor());
    }
    
}
