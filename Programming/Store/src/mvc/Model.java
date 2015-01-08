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
    public abstract Object getId();
    
    protected int getNewId() {
        Repository repo = Application.getRepository(this.getClass().getSimpleName().substring(0, 1).toLowerCase() + this.getClass().getSimpleName().substring(1) + "s");
        
        if ( repo == null )
            return 1;
        
        ModelCollection<Model> col = repo.all();
        
        int max = 0;
        
        if ( col == null || col.size() == 0 )
            return 1;
        
        for ( Model m : col ) {
            Integer saleId = (Integer) m.getId();
            if ( saleId > max )
                max = saleId;
        }
        
        return max + 1;
    }

    public boolean valid() {
        return true;
    }
    
    public Repository getRepository() {
        Repository repo;
        String className = this.getClass().getSimpleName();
        String repoKey = className.substring(0, 1).toLowerCase() + className.substring(1) + "s";
        repo = Application.getRepository(repoKey);
        
        if ( repo != null && repo.contains(this) )
            return repo;
        
        return null;
    }
}
