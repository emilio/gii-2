/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.views;

import mvc.Arguments;
import mvc.InvalidActionException;
import mvc.InvalidAppPathException;
import mvc.InvalidControllerException;
import mvc.Menu;
import mvc.View;

/**
 *
 * @author emilio
 */
public class SalesMenuView extends View {
    public SalesMenuView(Arguments a) {
        super(a);
    }

    @Override
    public void render() {
        Menu menu = new Menu("Sales");
        Arguments a;
        
        menu.addEntry("clients.index", "Show clients");
        menu.addEntry("sales.create", "New sale");
        menu.addEntry("sales.destroy", "Delete sale");
        menu.addEntry("sales.edit", "Edit sale");
        menu.addEntry("sales.index", "Show sales by client dni");
        
        a = new Arguments();
        a.put("orderBy", "value");
        menu.addEntry("sales.index", a, "Show sales by total value");
        
        try {
            while(menu.render().getResponse().execute()) {}
        } catch (InvalidAppPathException | InvalidControllerException | InvalidActionException ex) {
            ex.printStackTrace();
        }
    }
    
    
}
