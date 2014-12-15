/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import mvc.Arguments;
import mvc.Controller;

/**
 *
 * @author emilio
 */
public class MainController extends Controller {
    public void index(Arguments arg) {
        System.out.println("Hola!");
    }
}
