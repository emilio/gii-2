/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvc;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

/**
 *
 * @author emilio
 */
public class Application {
    private static Application mainInstance = null;

    static Application getMainInstance() {
        return mainInstance;
    }

    
    private static Map<String, Repository> repositories;
    
    public static void addRepository(String key, Repository repo) {
        if ( repositories == null )
            repositories = new HashMap<>();
        
        repositories.put(key, repo);
    }
    
    public static Repository getRepository(String key) {
        if ( repositories == null )
            return null;
        
        return repositories.get(key);
    }

    
    private final String classPath;
    
    public Application(String classPath) {
        this.classPath = classPath;
        if ( Application.mainInstance == null )
            Application.mainInstance = this;
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
        Object ret;
        Method action;
        View view;
        Redirection red;
        if ( parts.length != 2 )
            throw new InvalidAppPathException("Path " + path + " (length: " + parts.length + ") is invalid");
        
        try {
            controllerName = parts[0].substring(0, 1).toUpperCase() + parts[0].substring(1) + "Controller";
            controllerClass = Class.forName(classPath + ".controllers." 
                + controllerName);
        
            controller = (Controller) controllerClass.newInstance();
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException ex) {
            throw new InvalidControllerException("Controller " + controllerName + " was not found");
        }
        
        
        try {
            action = controllerClass.getMethod(parts[1], Arguments.class);
            ret = action.invoke(controller, args);
        } catch (InvocationTargetException | NoSuchMethodException | IllegalAccessException | SecurityException | IllegalArgumentException ex) {
            ex.printStackTrace();
            throw new InvalidActionException("Action " + parts[1] + " is not valid.");
            
        }
        
        if ( ret instanceof View )
            ((View) ret).render();
        else if ( ret instanceof Redirection )
            this.execute(((Redirection) ret).getRoute(), ((Redirection) ret).getArguments());
    }
}
