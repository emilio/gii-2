/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.repositories;

import store.models.Sale;
import mvc.FileBasedRepository;

/**
 *
 * @author emilio
 */
public class SalesRepository extends FileBasedRepository<Sale> {
    public SalesRepository() {
        super("sales.bin");
        this.load();
    }
}
