/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.repositories;

import store.models.Client;
import mvc.FileBasedRepository;

/**
 *
 * @author emilio
 */
public class ClientsRepository extends FileBasedRepository<Client> {
    public ClientsRepository() {
        super("clientes.bin");
        this.load();
    }
}
