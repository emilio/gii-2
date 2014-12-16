/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import mvc.Application;
import mvc.InvalidActionException;
import mvc.InvalidAppPathException;
import mvc.InvalidControllerException;

/**
 *
 * @author emilio
 */
public class Library {
    /**
     * @param args the command line arguments
     * @throws mvc.InvalidActionException
     */
    public static void main(String[] args) throws InvalidActionException, InvalidControllerException, InvalidAppPathException  {
        Application.addRepository("clients", new ClientsRepository());
        Application app = new Application("library");
        
        app.execute("main.index", null);
    }
}
