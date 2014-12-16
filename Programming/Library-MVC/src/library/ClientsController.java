/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import mvc.Application;
import mvc.Arguments;
import mvc.Controller;
import mvc.View;

/**
 *
 * @author emilio
 */
public class ClientsController extends Controller {
    public View index(Arguments a) {
        return new ClientsIndexView(a);
    }
    
    public View seed(Arguments a) {
        ClientsRepository repo = (ClientsRepository) Application.getRepository("clients");
        repo.add(new Client("70912324N", "Emilio Cobos", "C/ Mi casa, número mío"));
        repo.save();
        return null;
    }
}
