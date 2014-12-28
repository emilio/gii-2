/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.repositories;

import store.models.Refill;
import mvc.FileBasedRepository;

/**
 *
 * @author emilio
 */
public class RefillsRepository extends FileBasedRepository<Refill> {
    public RefillsRepository() {
        super("recambios.bin");
        this.load();
    }
}
