/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.views;

import store.models.Provider;
import mvc.Arguments;
import mvc.ModelCollection;
import mvc.View;

/**
 *
 * @author emilio
 */
public class ProvidersIndexView extends View {
    public ProvidersIndexView() {
        super();
    }
    
    public ProvidersIndexView(Arguments args) {
        super(args);
    }

    @Override
    public void render() {
        ModelCollection<Provider> col = (ModelCollection<Provider>) args.get("providers");
        
        if ( col == null || col.size() == 0 ) {
            System.out.println("No providers found");
            return;
        }
        
        for ( Provider p : col )
            System.out.println(p);
        
        System.out.println("Total records: " + col.size());
    }
    
}
