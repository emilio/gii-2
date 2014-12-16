/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

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

    MainIndexView(Arguments args) {
        super(args);
    }
    
    MainIndexView() {
        super();
    }

    @Override
    public void render() {
        Menu menu = new Menu("Main menu");
        menu.addEntry("main.hello", "Say Hello");
        menu.addEntry("clients.index", "Show clients");
        menu.addEntry("clients.seed", "Add a default client");
        
        try {
            while ( menu.render().getResponse().execute() ) {}
        } catch (InvalidAppPathException | InvalidControllerException | InvalidActionException ex) {
            ex.printStackTrace();
        }
    }
    
}
