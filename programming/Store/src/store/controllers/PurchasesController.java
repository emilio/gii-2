/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.controllers;

import java.util.Collections;
import java.util.Scanner;
import store.comparators.PurchaseProviderComparator;
import store.comparators.PurchaseValueComparator;
import store.models.Purchase;
import store.repositories.PurchasesRepository;
import store.views.PurchasesIndexView;
import store.views.PurchasesMenuView;
import mvc.Application;
import mvc.Arguments;
import mvc.Controller;
import mvc.ModelCollection;
import mvc.View;

/**
 *
 * @author emilio
 */
public class PurchasesController extends Controller {
    public View menu(Arguments a) {
        return new PurchasesMenuView(a);
    }
    
    public View index(Arguments a) {
        String orderBy = (String) a.get("orderBy");
        ModelCollection<Purchase> c = Application.getRepository("purchases").all();

        if ( orderBy == null || orderBy.equals("providerName") )
            Collections.sort(c, new PurchaseProviderComparator());
        else if ( orderBy.equals("value") )
            Collections.sort(c, new PurchaseValueComparator());
        else
            System.out.printf("Not recognized order: %s%n", orderBy);
        
        a.put("purchases", c);
        
        return new PurchasesIndexView(a);
    }
    
    public View create(Arguments a) {
        PurchasesRepository repo = (PurchasesRepository) Application.getRepository("purchases");
        Scanner sc = new Scanner(System.in);
        
        System.out.print("Introduce provider id: ");
        String providerId = sc.nextLine();
        
        System.out.print("Introduce refill id: ");
        String refillId = sc.nextLine();
        
        System.out.print("Introduce count: ");
        int count = sc.nextInt();
        
        Purchase s = new Purchase(providerId, refillId, count);
        
        if ( ! s.valid() ) {
            System.out.println("Invalid purchase: " + s.getErrorMessage());
            return null;
        }
        
        repo.add(s);
        repo.save();
                
        s.getRefill().addStock(s.getCount());
        System.out.println(s.getRefill());
        System.out.println(s.getRefill().getRepository());
        s.getRefill().getRepository().save();
        
        System.out.println("Purchase correctly added: ");
        System.out.println(s);
        
        return null;
    }
    
    public View destroy() {
        PurchasesRepository repo = (PurchasesRepository) Application.getRepository("purchases");
        Scanner sc = new Scanner(System.in);
        
        System.out.print("Introduce purchase id: ");
        int id = sc.nextInt();
        
        Purchase p = repo.all().findFirstBy("id", id);
        
        if ( p == null ) {
            System.out.println("Purchase doesn't exist");
            return null;
        }
        
        if ( p.getRefill() == null ) {
            System.out.println("Refill could not be updated, it doesn't exist");
        } else if ( p.getRefill().getStock() < p.getCount() ) {
            System.out.println("Not enough stock to delete purchase.");
            return null;
        } else {
            p.getRefill().addStock(-p.getCount());
            p.getRefill().getRepository().save();
        }
        
        repo.delete(p);
        repo.save();
        
        return null;
    }
}
