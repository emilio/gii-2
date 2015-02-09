/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package quiniela;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author emilio
 */
public class Quiniela {
    
    /** if invalid we output to stdout */
    public static final String OUTPUT_FILE_NAME = "results.txt";
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        ArrayList<Match> matches = new ArrayList<>();
        Map<Match, Character> results = new HashMap<>();
        Scanner sc = new Scanner(System.in);
        
        matches = seed(matches);
        
        System.out.println("Introduce 1, x o 2:");
        
        for ( Match m: matches ) {
            results.put(m, getResult(m, sc));
        }
        
        try {
            write(results);
        } catch (FileNotFoundException | UnsupportedEncodingException ex) {
            System.out.println("Error escribiendo los resultados.");
        }
    }

    private static ArrayList<Match> seed(ArrayList<Match> matches) {
        matches.add(new Match("Córdoba", "Málaga"));
        matches.add(new Match("At. Madrid", "Espanyol"));
        matches.add(new Match("Elche", "Sevilla"));
        matches.add(new Match("Villareal", "Almería"));
        matches.add(new Match("Athletic Club", "Celta de Vigo"));
        matches.add(new Match("Levante", "Real Madrid"));
        matches.add(new Match("Barcelona", "Eibar"));
        matches.add(new Match("Alavés", "Lugo"));
        matches.add(new Match("Zaragoza", "Racing"));
        matches.add(new Match("Osasuna", "Tenerife"));
        matches.add(new Match("Leganés", "Betis"));
        matches.add(new Match("Valladolid", "Ponferradina"));
        matches.add(new Match("Mirandés", "Albacete"));
        matches.add(new Match("Sabadell", "Girona"));
        matches.add(new Match("Deportivo", "Valencia"));
        
        return matches;
    }

    private static Character getResult(Match match, Scanner sc) {
        String line;
        char ret = 0;
        do {
            System.out.print(match + ": ");
            line = sc.nextLine();
            
            if ( line.length() != 0 ) {
                ret = line.charAt(0);
            }
            
            if ( ret == '1' || ret == '2' || ret == 'x' ) {
                break;                
            }
            
            System.out.println("Resultado no válido...");
        } while( true );
        
        return ret;
    }

    
    private static void write(Map<Match, Character> results) throws FileNotFoundException, UnsupportedEncodingException {
        File file = new File(OUTPUT_FILE_NAME);
        PrintWriter writer = new PrintWriter(file, "UTF-8");

        /* Podría estar hecho de una forma mucho más chula, calculando la longitud máxima y tal pero... no hay tiempo! */
        writer.println("+-----------------------+");
        for ( Map.Entry<Match, Character> entry : results.entrySet() ) {
            writer.println("| " + entry.getKey() + ": " + entry.getValue() + " |");
            writer.println("+-----------------------+");
        }
        
        writer.close();
    }
}
