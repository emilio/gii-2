/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.views;

import store.models.Client;
import mvc.Application;
import mvc.Arguments;
import mvc.ModelCollection;
import mvc.View;

/**
 *
 * @author emilio
 */
public class ClientsIndexView extends View {

    public ClientsIndexView(Arguments a) {
        super(a);
    }
    
    public ClientsIndexView() {
        super();
    }

    @Override
    public void render() {
        ModelCollection<Client> col = (ModelCollection<Client>) args.get("clients");
        
        if ( col == null || col.size() == 0 ) {
            System.out.println("No clients found");
            return;
        }
            
        for ( Client c : col )
            System.out.println(c);
        
        System.out.println("Total records: " + col.size());
    }
    
}
