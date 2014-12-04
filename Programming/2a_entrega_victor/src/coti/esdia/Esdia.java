/*

Version history

26/11/13 07:57

Fixed problem with diacritics. Esdia will now choose the iso-8859-1 Charset for
Windows OSes, and UTF-8 for non-Windows OSes. This lets you input commands
with diacritics (áéíóú).

*/
//
// ES IMPORTANTE INCLUIR EL PAQUETE:
//
package coti.esdia;

import java.util.Scanner;

public class Esdia {

    final public static Scanner sc = new Scanner(System.in,
            System.getProperty("os.name").contains("Windows") ? "iso-8859-1":"UTF-8");

    static public float readFloat(String prompt) {
        float tempNumber = 0;
        String temp;
        boolean numberOkay = true;
        do {
            numberOkay = false;
            do {
                System.out.printf("%s ", prompt);
                temp = sc.nextLine();

            } while (temp.isEmpty());
            try {
                tempNumber = Float.parseFloat(temp);
                numberOkay = true;
            } catch (Exception e) {
                System.out.printf("%nPerdón, ese número no es correcto. Pruebe de nuevo.%n");
                numberOkay = false;
            }
        } while (!numberOkay);
        return tempNumber;
    }
    
    static public double readDouble(String prompt) {
        double tempNumber = 0;
        String temp;
        boolean numberOkay = true;
        do {
            numberOkay = false;
            do {
                System.out.printf("%s ", prompt);
                temp = sc.nextLine();

            } while (temp.isEmpty());
            try {
                tempNumber = Double.parseDouble(temp);
                numberOkay = true;
            } catch (Exception e) {
                System.out.printf("%nPerdón, ese número no es correcto. Pruebe de nuevo.%n");
                numberOkay = false;
            }
        } while (!numberOkay);
        return tempNumber;
    }

   static public double readDouble(String prompt, double min, double max) {
        double tempNumber = 0;
        String temp;
        boolean numberOkay = true;
        do {
            numberOkay = false;
            do {
                System.out.printf("%s (%f <= numero <= %f)", prompt, min, max);
                temp = sc.nextLine();

            } while (temp.isEmpty());
            try {
                tempNumber = Double.parseDouble(temp);
                numberOkay = true;
            } catch (Exception e) {
                System.out.printf("%nPerdón, ese número no es correcto. Pruebe de nuevo.%n");
                numberOkay = false;
            }
            if(tempNumber < min || tempNumber > max)
            {
            	System.out.printf("%n%nPor favor, escriba un número entre %f y %f%n%n",min,max);
            	numberOkay = false;
            }
        } while (!numberOkay);
        return tempNumber;
    }

    static public int readInt(String prompt) {
        int tempNumber = 0;
        String temp;
        boolean numberOkay = true;
        do {
            numberOkay = false;
            do {
                System.out.printf("%s ", prompt);
                temp = sc.nextLine();

            } while (temp.isEmpty());
            try {
                tempNumber = Integer.parseInt(temp);
                numberOkay = true;
            } catch (Exception e) {
                System.out.printf("%nPerdón, ese número no es correcto. Pruebe de nuevo.%n");
                numberOkay = false;
            }
        } while (!numberOkay);
        return tempNumber;
    }
    
   static public int readInt(String prompt, int min, int max) {
        int tempNumber = 0;
        String temp;
        boolean numberOkay = true;
        do {
            numberOkay = false;
            do {
                System.out.printf("%s (%d <= numero <= %d)", prompt, min, max);
                temp = sc.nextLine();

            } while (temp.isEmpty());
            try {
                tempNumber = Integer.parseInt(temp);
                numberOkay = true;
            } catch (Exception e) {
                System.out.printf("%nPerdón, ese número no es correcto. Pruebe de nuevo.%n");
                numberOkay = false;
            }
            if(tempNumber < min || tempNumber > max)
            {
            	System.out.printf("%n%nPor favor, escriba un número entre %d y %d%n%n",min,max);
            	numberOkay = false;
            }
        } while (!numberOkay);
        return tempNumber;
    }

    static public String readString(String prompt) {
        String temp = null;
        System.out.printf("%s ", prompt);
        temp = sc.nextLine();
        return temp;

    }

    static public String readString_ne(String prompt) {
        String temp = null;
        do {
            System.out.printf("%s ", prompt);
            temp = sc.nextLine();
            if (temp.isEmpty()) {
                System.out.printf("Error: este campo no puede estar vacío.");
            }
        } while (temp.isEmpty());
        return temp;

    }

    static public String readString(String prompt, String op1, String op2) {
        String temp = null;
        boolean badOption;
        do {
            System.out.printf("%s (%s,%s)", prompt, op1, op2);
            temp = sc.nextLine();
            boolean tempIsNotOp1 = temp.compareToIgnoreCase(op1) != 0;
            boolean tempIsNotOp2 = temp.compareToIgnoreCase(op2) != 0;
            badOption = tempIsNotOp1 && tempIsNotOp2;
            if (badOption) {
                System.err.printf("%nOpción incorrecta: Por favor, escriba %s o %s%n%n", op1, op2);
            }
        } while (badOption);
        return temp;
    }
}