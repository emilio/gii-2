/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvc;

/**
 *
 * @author emilio
 */
public final class Redirection {
    private final String route;
    private final Arguments arguments;
    
    public String getRoute() {
        return route;
    }
    
    public Arguments getArguments() {
        return arguments;
    }
    
    public Redirection(String route, Arguments arguments) {
        this.route = route;
        this.arguments = arguments;
    }
    
    public Redirection(String route) {
        this.route = route;
        this.arguments = new Arguments();
    }
}
