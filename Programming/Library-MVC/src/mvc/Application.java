/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvc;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 *
 * @author emilio
 */
public class Application {
    private final String classPath;
    
    public Application(String classPath) {
        this.classPath = classPath;
    }
    
    /** 
     * @param path of the form "controller.action"
     * @param args
     * @throws mvc.InvalidAppPathException
     * @throws mvc.InvalidControllerException
     * @throws mvc.InvalidActionException
     */
    public void execute(String path, Arguments args) throws InvalidAppPathException, InvalidControllerException, InvalidActionException {
        String[] parts = path.split("\\.");
        String controllerName = null;
        Class controllerClass;
        Controller controller;
        Method action;
        View view;
        if ( parts.length != 2 )
            throw new InvalidAppPathException("Path " + path + " (length: " + parts.length + ") is invalid");
        
        try {
            controllerName = parts[0].substring(0, 1).toUpperCase() + parts[0].substring(1) + "Controller";
            controllerClass = Class.forName(classPath + "." 
                + controllerName);
        
            controller = (Controller) controllerClass.newInstance();
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException ex) {
            throw new InvalidControllerException("Controller " + controllerName + " was not found");
        }
        
        
        try {
            action = controllerClass.getMethod(parts[1], Arguments.class);
            view = (View) action.invoke(controller, args);
        } catch (InvocationTargetException | NoSuchMethodException | IllegalAccessException | SecurityException | IllegalArgumentException ex) {
            throw new InvalidActionException("Action " + parts[1] + " is not valid.");
        }
    }
}
