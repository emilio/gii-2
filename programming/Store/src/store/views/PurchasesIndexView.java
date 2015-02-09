/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.views;

import store.models.Purchase;
import mvc.Arguments;
import mvc.ModelCollection;
import mvc.View;

/**
 *
 * @author emilio
 */
public class PurchasesIndexView extends View {
    
    public PurchasesIndexView(Arguments a) {
        super(a);
    }

    @Override
    public void render() {
        ModelCollection<Purchase> col = (ModelCollection<Purchase>) args.get("purchases");
     
        for ( Purchase s : col )
            System.out.println(s);
        
        System.out.println("Total records: " + col.size());
    }
        
}
