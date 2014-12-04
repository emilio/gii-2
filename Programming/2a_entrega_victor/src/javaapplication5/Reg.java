package javaapplication5;

import static coti.esdia.Esdia.readFloat;
import static coti.esdia.Esdia.readInt;
import static coti.esdia.Esdia.readString;
import java.io.Serializable;
import static javaapplication5.Menu.flag;

/**
 *
 * @author Victor Barrueco <vbarruec@usal.es>
 */
public class Reg implements Serializable, Comparable {
    String fabricante;
    String modelo; 
    String color;
    Float grosor;
    Float precio;
    int numUnidades;
    
    public Reg(String fabricante, String modelo, String color, Float grosor, Float precio, int numUnidades){
        this.fabricante = fabricante;
        this.modelo = modelo;
        this.color = color;
        this.grosor = grosor;
        this.precio = precio;
        this.numUnidades = numUnidades;
    }

    Reg() {
    }
    
    /**************************************************************************/
    /* METODO PARA AÑADIR                                                     */
    /**************************************************************************/
    void add() {
        this.fabricante = readString("Escriba el nombre del fabricante...: ");
        this.modelo = readString("Escriba el nombre del modelo.......: ");
        this.color = readString("Escriba el color del rotulador.....: ");
        this.grosor = readFloat("Escriba el grosor del rotulador....: ");
        this.precio = readFloat("Escriba el precio del rotulador....: ");
        this.numUnidades = readInt("Escriba el numero de unidades......: ");
    }
    
    /**************************************************************************/
    /* METODOS GET                                                            */
    /**************************************************************************/
    /**
     *
     * @return
     */
    public String getFabricante(){
        return this.fabricante;
    }
    public String getModelo(){
        return this.modelo;
    }
    public String getColor(){
        return this.color;
    }
    public Float getGrosor(){
        return this.grosor;
    }
    public Float getPrecio(){
        return this.precio;
    }
    public int getNumUnidades(){
        return this.numUnidades;
    }    
    
    public int compareTo(Object o1) {
            
        int cmpValue = 0;
        
        if(flag == 1){
            if ((cmpValue = this.fabricante.compareToIgnoreCase(((Reg) o1).fabricante)) == 0) {
                if ((cmpValue = this.modelo.compareToIgnoreCase(((Reg) o1).modelo)) == 0) {
                    if ((cmpValue = this.color.compareToIgnoreCase(((Reg) o1).color)) == 0) {
                        if ((cmpValue = (this.grosor).compareTo(((Float)((Reg) o1).grosor))) == 0) {
                            if ((cmpValue = ((Float) this.precio).compareTo(((Float)((Reg) o1).precio))) == 0) {
                                if ((cmpValue = ((Integer) this.numUnidades).compareTo(((Integer)((Reg) o1).numUnidades))) == 0) {
                
                                        return 0; 
                                      
                                }
                            }
                        }
                    }
                }    
            }        
        }   
        
        if(flag == 2){
            
                if ((cmpValue = this.modelo.compareToIgnoreCase(((Reg) o1).modelo)) == 0) {
                    if ((cmpValue = this.color.compareToIgnoreCase(((Reg) o1).color)) == 0) {
                        if ((cmpValue = ((Float) this.grosor).compareTo(((Float)((Reg) o1).grosor))) == 0) {
                            if ((cmpValue = ((Float) this.precio).compareTo(((Float)((Reg) o1).precio))) == 0) {
                                if ((cmpValue = ((Integer) this.numUnidades).compareTo(((Integer)((Reg) o1).numUnidades))) == 0) {
                                  
                                        return 0; 
                                  
                                }
                            }
                        }
                    }
                }    
                    
        }
        
        if(flag == 3){

                    if ((cmpValue = this.color.compareToIgnoreCase(((Reg) o1).color)) == 0) {
                        if ((cmpValue = ((Float) this.grosor).compareTo(((Float)((Reg) o1).grosor))) == 0) {
                            if ((cmpValue = ((Float) this.precio).compareTo(((Float)((Reg) o1).precio))) == 0) {
                                if ((cmpValue = ((Integer) this.numUnidades).compareTo(((Integer)((Reg) o1).numUnidades))) == 0) {
                         
                                        return 0; 
                                        
                                }
                            }
                        }
                    }
        
        }
        
        if(flag == 4){

                        if ((cmpValue = ((Float) this.grosor).compareTo(((Float)((Reg) o1).grosor))) == 0) {
                            if ((cmpValue = ((Float) this.precio).compareTo(((Float)((Reg) o1).precio))) == 0) {
                                if ((cmpValue = ((Integer) this.numUnidades).compareTo(((Integer)((Reg) o1).numUnidades))) == 0) {
              
                                        return 0; 
                                       
                                }
                            }
                        }
       
        }
        
        if(flag == 5){

                            if ((cmpValue = ((Float) this.precio).compareTo(((Float)((Reg) o1).precio))) == 0) {
                                if ((cmpValue = ((Integer) this.numUnidades).compareTo(((Integer)((Reg) o1).numUnidades))) == 0) {
                                  
                                        return 0; 
                                 
                                }
                            }
       
        }
        
        if(flag == 6){

                                if ((cmpValue = ((Integer) this.numUnidades).compareTo(((Integer)((Reg) o1).numUnidades))) == 0) {

                                        return 0; 
                              
                                }
       
        }
        

        
        return cmpValue;
    }
}