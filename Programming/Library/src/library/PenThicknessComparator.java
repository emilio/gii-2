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
public class PenThicknessComparator implements Comparator <Pen> {

    @Override
    public int compare(Pen o1, Pen o2) {
        Float thickness1 = o1.getThickness();
        Float thickness2 = o2.getThickness();
        return thickness1.compareTo(thickness2);
    }
    
    
}
