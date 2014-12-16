/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvc;

import java.io.Serializable;

/**
 *
 * @author emilio
 */
public abstract class Model implements Serializable {
    public boolean valid() {
        return true;
    }
}
