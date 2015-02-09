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
public class MainIndexView extends View {

    public MainIndexView(Arguments args) {
        super(args);
    }
    
    public MainIndexView() {
        super();
    }

    @Override
    public void render() {
        Menu menu = new Menu("Main menu");
        menu.addEntry("sales.menu", "Sales");
        menu.addEntry("purchases.menu", "Purchases");
        menu.addEntry("refills.index", "Show refills");
        
        menu.addEntry("clients.importFromFile",   "Import clients");
        menu.addEntry("providers.importFromFile", "       providers");
        menu.addEntry("refills.importFromFile",   "       refills");

        try {
            while ( menu.render().getResponse().execute() ) {}
        } catch (InvalidAppPathException | InvalidControllerException | InvalidActionException ex) {
            ex.printStackTrace();
        }
    }
    
}
