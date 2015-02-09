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
public class PurchasesMenuView extends View {
    public PurchasesMenuView(Arguments a) {
        super(a);
    }

    @Override
    public void render() {
        Menu menu = new Menu("Purchases");
        Arguments a;
        
        menu.addEntry("providers.index", "Show providers");
        menu.addEntry("purchases.create", "Create purchase");
        menu.addEntry("purchases.destroy", "Delete purchase");
        menu.addEntry("purchases.index", "Show purchases by provider id");
        a = new Arguments();
        a.put("orderBy", "value");
        menu.addEntry("purchases.index", a, "Show purchases by purchase value");
        
        try {
            while(menu.render().getResponse().execute()) {}
        } catch (InvalidAppPathException | InvalidControllerException | InvalidActionException ex) {
            ex.printStackTrace();
        }
    }
    
    
}
