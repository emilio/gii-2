/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.repositories;

import store.models.Purchase;
import mvc.FileBasedRepository;

/**
 *
 * @author emilio
 */
public class PurchasesRepository extends FileBasedRepository<Purchase> {
    public PurchasesRepository() {
        super("purchases.bin");
        this.load();
    }
}
