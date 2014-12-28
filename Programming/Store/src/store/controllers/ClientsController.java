/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.controllers;

import store.models.Client;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;
import store.comparators.ClientComparator;
import store.views.ClientsIndexView;
import store.repositories.ClientsRepository;
import mvc.Application;
import mvc.Arguments;
import mvc.Controller;
import mvc.ModelCollection;
import mvc.View;

/**
 *
 * @author emilio
 */
public class ClientsController extends Controller {
    public View index(Arguments a) {
        ModelCollection<Client> c = Application.getRepository("clients").all();
        
        do {
            Scanner sc = new Scanner(System.in);
            String dni;
            System.out.print("Filter by dni (empty to show all):");
            dni = sc.nextLine().trim();
            if ( ! dni.isEmpty() )
                c = c.findBy("dni", dni);
        } while ( false );
        
        Collections.sort(c, new ClientComparator());

        a.put("clients", c);
        return new ClientsIndexView(a);
    }
    
    private int _import(File f) {
        ClientsRepository repo = (ClientsRepository) Application.getRepository("clients");
        FileInputStream fis;
        DataInputStream dis;
        // This way we import everything, later if succeed we add all records
        ArrayList<Client> clients;
        int count;
        try {
            fis = new FileInputStream(f);
            dis = new DataInputStream(fis);
            count = dis.readInt();
            System.out.printf("Ready to import %d clients%n", count);
            clients = new ArrayList<>(count);
            while ( count-- > 0 ) {
                String dni = dis.readUTF();
                String dir = dis.readUTF();
                String name = dis.readUTF();
                clients.add(new Client(dni, name, dir));
            }
            
            dis.close();
            fis.close();
        } catch ( IOException ex ) {
            ex.printStackTrace();
            return 0;
        }
        
        for ( Client c: clients )
            repo.add(c);
        
        repo.save();
        
        return clients.size();
    }
    
    public View importFromFile(Arguments a) {
        Scanner sc = new Scanner(System.in);
        File f;
        String fileName;
        int imported;
        
        while ( true ) {
            System.out.print("Introduce file path to import (\"cancel\" to abort): ");
            fileName = sc.nextLine();
            
            if ( fileName.equalsIgnoreCase("cancel") )
                return null;
            
            f = new File(fileName);
            
            if ( f.exists() && ! f.isDirectory() )
                break;
            else
                System.out.println("File does not exist or is a directory.");
        }

        imported = _import(f);
        
        if ( imported > 0 )
            System.out.printf("Import succeded, imported %d clients%n", imported);
        else
            System.out.println("Import failed, check file format");
        
        return null;
    }
}
