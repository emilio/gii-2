package matrixdeterminant;

import java.util.Scanner;

/**
 *
 * @author Emilio Cobos < emiliocobos@usal.es >
 */
public class MatrixDeterminant {
    
    private static int [][] matrix;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        if ( args.length == 0 ) {
            getMatrixFromKeyBoard();
        } else {
            getMatrixFromArgs(args);
        }
        
        System.out.println("Dimension of matrix is: " + matrix.length);
        printMatrix();
        System.out.println("Determinant is: " + getDeterminant());
    }
    
    public static void printMatrix() {
        for ( int i = 0; i < matrix.length; i++ ) {
            for ( int j = 0; j < matrix.length; j++ ) {
                System.out.printf("\t%d", matrix[i][j]);
            }
            System.out.printf("\n");
        }    
    }

    private static int getDeterminant() {
        switch ( matrix.length ) {
            case 1:
                return matrix[0][0];
            case 2:
                return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
            case 3:
                return matrix[0][0] * matrix[1][1] * matrix[2][2] 
                     + matrix[0][1] * matrix[1][2] * matrix[2][0]
                     + matrix[1][0] * matrix[2][1] * matrix[0][2]
                     - matrix[0][2] * matrix[1][1] * matrix[2][0]
                     - matrix[1][0] * matrix[0][1] * matrix[2][2]
                     - matrix[1][2] * matrix[2][1] * matrix[0][0];
            default:
                throw new UnsupportedOperationException("Not yet implemented");
        }
    }

    private static void getMatrixFromKeyBoard() {
        Scanner sc = new Scanner(System.in);
        int dimension;
        
        do {
            System.out.println("Introduce la dimensión de la matriz: ");
            dimension = sc.nextInt();
        } while ( dimension <= 0 );
        
        matrix = new int[dimension][dimension];
        
        for ( int i = 0; i < dimension; i++ ) {
            for ( int j = 0; j < dimension; j++ ) {
                System.out.printf("Introduce elemento (%d, %d):%n", i, j);
                matrix[i][j] = sc.nextInt();
            }
        }
        
    }

    private static void getMatrixFromArgs(String[] args) {
        double square = Math.sqrt(args.length);
    
        // If it's not a perfect square
        if ( ((int) square * square) != args.length ) {
            throw new IllegalArgumentException("La matriz debe de ser cuadrada");       
        }
        
        int dimension = (int) square;
        
        matrix = new int[dimension][dimension];
        
        for ( int i = 0; i < args.length; i++ ) {
            matrix[i / dimension][i % dimension] = Integer.parseInt(args[i]);
        }
    }
}