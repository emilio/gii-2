/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import mvc.FileBasedRepository;

/**
 *
 * @author emilio
 */
public class ClientsRepository extends FileBasedRepository<Client> {
    private final String fileName = "clientes.bin";
    
    public ClientsRepository() {
        this.load();
    }
}
