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
public abstract class View {
    public final Arguments args;
    
    
    
    public View() {
        this.args = new Arguments();
    }
    
    public View(Arguments args) {
        if ( args == null )
            args = new Arguments();

        this.args = args;
    }
    
    public abstract void render();
}
