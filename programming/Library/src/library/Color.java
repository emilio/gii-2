/* Simple color wrapper, currently accepting just rgb */
package library;

/**
 *
 * @author emilio
 */
final class Color {
    private final int r;
    private final int g;
    private final int b;
    // private final int a; // alpha channel
    
    // We should add hex support and all that stuff
    Color(int r, int g, int b) {
        this.r = r;
        this.g = g;
        this.b = b;
    }
    
    @Override
    public String toString() {
        return "rgb(" + r + ", " + g + ", " + b + ")";
    }
}
