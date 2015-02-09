/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package store.controllers;

import java.util.Collections;
import java.util.Scanner;
import store.comparators.SaleClientComparator;
import store.comparators.SaleValueComparator;
import store.models.Sale;
import store.repositories.SalesRepository;
import store.views.SalesIndexView;
import store.views.SalesMenuView;
import mvc.Application;
import mvc.Arguments;
import mvc.Controller;
import mvc.ModelCollection;
import mvc.View;

/**
 *
 * @author emilio
 */
public class SalesController extends Controller {
    public View menu(Arguments a) {
        return new SalesMenuView(a);
    }
    
    public View index(Arguments a) {
        String orderBy = (String) a.get("orderBy");
        ModelCollection<Sale> c = Application.getRepository("sales").all();

        if ( orderBy == null || orderBy.equals("providerName") )
            Collections.sort(c, new SaleClientComparator());
        else if ( orderBy.equals("value") )
            Collections.sort(c, new SaleValueComparator());
        else
            System.out.printf("Not recognized order: %s%n", orderBy);
        
        a.put("sales", c);
        
        return new SalesIndexView(a);
    }
    
    public View create(Arguments a) {
        SalesRepository repo = (SalesRepository) Application.getRepository("sales");
        Scanner sc = new Scanner(System.in);
        
        System.out.print("Introduce client id: ");
        String clientId = sc.nextLine();
        
        System.out.print("Introduce refill id: ");
        String refillId = sc.nextLine();
        
        System.out.print("Introduce count: ");
        int count = sc.nextInt();
        
        Sale s = new Sale(clientId, refillId, count);
        
        if ( ! s.valid() ) {
            System.out.println("Invalid sale: " + s.getErrorMessage());
            return null;
        }
        
        repo.add(s);
        repo.save();
        
        
        s.getRefill().addStock(-s.getCount());
        s.getRefill().getRepository().save();

        
        System.out.println("Sale correctly added: ");
        System.out.println(s);
        
        return null;
    }
    
    public View destroy(Arguments a) {
        SalesRepository repo = (SalesRepository) Application.getRepository("sales");
        Scanner sc = new Scanner(System.in);
        
        System.out.print("Introduce sale id: ");
        int id = sc.nextInt();
        
        Sale p = repo.all().findFirstBy("id", id);
        
        if ( p == null ) {
            System.out.println("Sale doesn't exist");
            return null;
        }
        
        // Refill stock
        if ( p.getRefill() != null ) {
            p.getRefill().addStock(p.getCount());
            p.getRefill().getRepository().save();
        }
        
        repo.delete(p);
        repo.save();
        
        return null;
    }
    
    public View edit(Arguments a) {
        SalesRepository repo = (SalesRepository) Application.getRepository("sales");
        ModelCollection<Sale> sales = repo.all();
        Scanner sc = new Scanner(System.in);
        
        System.out.print("Introduce sale id: ");
        int id = sc.nextInt();
        
        Sale s = sales.findFirstBy("id", id);
        if ( s == null ) {
            System.out.println("Sale doesn't exist");
            return null;
        }
        
        String previousClientId = s.getClientId();
        System.out.printf("Client id (%s): ", previousClientId);
        String clientId = sc.nextLine();
        
        if ( ! clientId.isEmpty() ) {
            s.setClientId(clientId);
            if ( s.getClient() == null ) {
                System.out.println("Client id couldn't be updated: it's not a valid client.");
                s.setClientId(previousClientId);
            }
        }
        
        // temporarily restore stock, in case refill or count changes
        if ( s.getRefill() != null )
            s.getRefill().addStock(s.getCount());
        
        String previousRefillId = s.getRefillId();
        System.out.printf("Refill id (%s): ", previousRefillId);        
        String refillId = sc.nextLine();
        
        if ( ! refillId.isEmpty() ) {
            s.setRefillId(refillId);
            if ( s.getRefill() == null ) {
                System.out.println("Refill id couldn't be updated: it's not a valid refill.");
                s.setRefillId(previousRefillId);
            }
        }
        
        int previousCount = s.getCount();
        
        System.out.printf("Count (%d) (0 to skip):", previousCount);
        int count = Math.abs(sc.nextInt());

        if ( count != 0 )
            s.setCount(count);

        if ( s.getRefill() == null ) {
            System.out.println("Could not update refill, unexistent.");
        } else if ( s.getRefill().getStock() < s.getCount() ) {
            System.out.println("We have not enough stock, usign max.");
            s.setCount(s.getRefill().getStock());
            s.getRefill().setStock(0);
        } else {
            s.getRefill().addStock(-s.getCount());
        }
        
        s.getRefill().getRepository().save();
        repo.save();
        
        return null;
    }
}
