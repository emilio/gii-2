/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.controllers;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import store.comparators.ProviderComparator;
import store.models.Provider;
import store.repositories.ProvidersRepository;
import store.views.ProvidersIndexView;
import mvc.Application;
import mvc.Arguments;
import mvc.Controller;
import mvc.ModelCollection;
import mvc.View;


/**
 *
 * @author emilio
 */
public class ProvidersController extends Controller {
    public View index(Arguments a) {
        ModelCollection<Provider> c = Application.getRepository("providers").all();
        
        do {
            Scanner sc = new Scanner(System.in);
            String nif;
            System.out.print("Filter by nif (empty to show all): ");
            nif = sc.nextLine();
            if ( ! nif.isEmpty() )
                c = c.findBy("nif", nif);
        } while ( false );
        
        c.sort(new ProviderComparator());
        a.put("providers", c);
        return new ProvidersIndexView(a);
    }

    private int _import(File f) {
        ProvidersRepository repo = (ProvidersRepository) Application.getRepository("providers");
        FileInputStream fis;
        DataInputStream dis;
        // This way we import everything, later if succeed we add all records
        ArrayList<Provider> providers;
        int count;
        try {
            fis = new FileInputStream(f);
            dis = new DataInputStream(fis);
            count = dis.readInt();
            System.out.printf("Ready to import %d providers%n", count);
            providers = new ArrayList<>(count);
            while ( count-- > 0 ) {
                Provider p;
                String dir = dis.readUTF();
                String nif = dis.readUTF();
                String name = dis.readUTF();
                p = new Provider(nif, name, dir);
                providers.add(p);
            }
            
            dis.close();
            fis.close();
        } catch ( IOException ex ) {
            ex.printStackTrace();
            return 0;
        }
        
        for ( Provider c: providers )
            repo.add(c);
        
        repo.save();
        
        return providers.size();
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
            System.out.printf("Import succeded, imported %d providers%n", imported);
        else
            System.out.println("Import failed, check file format");
        
        return null;
    }
}
