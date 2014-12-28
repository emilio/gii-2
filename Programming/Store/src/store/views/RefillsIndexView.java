/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.views;

import store.models.Refill;
import mvc.Arguments;
import mvc.ModelCollection;
import mvc.View;

/**
 *
 * @author emilio
 */
public class RefillsIndexView extends View {

    public RefillsIndexView(Arguments a) {
        super(a);
    }

    @Override
    public void render() {
        ModelCollection<Refill> col = (ModelCollection<Refill>) args.get("refills");
        
        if ( col == null || col.size() == 0 ) {
            System.out.println("No refills found");
            return;
        }
        
        for ( Refill r : col )
            System.out.println(r);
        
        System.out.println("Total records: " + col.size());
        
    }
    
}
