/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import mvc.Arguments;
import mvc.Controller;
import mvc.View;

/**
 *
 * @author emilio
 */
public class MainController extends Controller {
    public View index(Arguments arg) {
        return new MainIndexView(arg);
    }
    
    public View hello(Arguments arg) {
        System.out.println("Hello!");
        return null;
    }
}
