/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.controllers;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import store.models.Refill;
import store.views.RefillsIndexView;
import store.repositories.RefillsRepository;
import mvc.Application;
import mvc.Arguments;
import mvc.Controller;
import mvc.View;

/**
 *
 * @author emilio
 */
public class RefillsController extends Controller {
    public View index(Arguments a) {
        a.put("refills", Application.getRepository("refills").all());
        return new RefillsIndexView(a);
    }
    
    private int _import(File f) {
        RefillsRepository repo = (RefillsRepository) Application.getRepository("refills");
        BufferedReader reader;
        // This way we import everything, later if succeed we add all records
        ArrayList<Refill> refills;
        int count;
        try {
            String line;
            reader = new BufferedReader(new FileReader(f));
            refills = new ArrayList<>();

            
            while ( (line = reader.readLine()) != null ) {
                String [] parts = line.split("\\s{2,}"); // Split by more than two spaces
                if ( parts.length != 4 ) {
                    System.out.printf("Bad line: \"%s\"%n", line);
                    continue;
                }
                
                String name = parts[0].trim();
                String id = parts[1].trim();
                
                // Note we don't use localization, we exactly know the format
                float price = Float.parseFloat(parts[2].trim().replace(",", "."));
                
                int stock = Integer.parseInt(parts[3].trim());
                refills.add(new Refill(id, name, price, stock));
            }
            
            reader.close();
        } catch ( IOException ex ) {
            ex.printStackTrace();
            return 0;
        }
        
        for ( Refill c: refills )
            repo.add(c);
        
        repo.save();
        
        return refills.size();
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
            System.out.printf("Import succeded, imported %d refills%n", imported);
        else
            System.out.println("Import failed, check file format");
        
        return null;
    }
    public View seed(Arguments a) {
        RefillsRepository repo = (RefillsRepository) Application.getRepository("refills");
        repo.add(new Refill("trc", "tuerca", (float) 2.3));
        repo.add(new Refill("ledb", "LED blanco", (float) 5));
        repo.save();
        return null;
    }
}
