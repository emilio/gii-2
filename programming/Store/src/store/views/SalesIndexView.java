/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.views;

import store.models.Sale;
import mvc.Arguments;
import mvc.ModelCollection;
import mvc.View;

/**
 *
 * @author emilio
 */
public class SalesIndexView extends View {
    
    public SalesIndexView(Arguments a) {
        super(a);
    }

    @Override
    public void render() {
        ModelCollection<Sale> col = (ModelCollection<Sale>) args.get("sales");
     
        for ( Sale s : col )
            System.out.println(s);
        
        System.out.println("Total records: " + col.size());
    }
        
}
