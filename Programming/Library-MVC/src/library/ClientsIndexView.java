/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import mvc.Application;
import mvc.Arguments;
import mvc.ModelCollection;
import mvc.View;

/**
 *
 * @author emilio
 */
public class ClientsIndexView extends View {

    ClientsIndexView(Arguments a) {
        super(a);
    }
    
    ClientsIndexView() {
        super();
    }

    @Override
    public void render() {
        ModelCollection<Client> col = Application.getRepository("clients").all();
        
        if ( col.size() == 0 ) {
            System.out.println("No clients found");
            return;
        }
            
        for ( Client c : col )
            System.out.println(c.getDni() + "\t" + c.getName() + "\t" + c.getDirection());
    }
    
}
