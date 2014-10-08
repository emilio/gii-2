/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package quiniela;

/**
 *
 * @author emilio
 */
public final class Match {
    private final String localTeam;
    private final String visitorTeam;
    
    public Match(String local, String visitor) {
        localTeam = local;
        visitorTeam = visitor;
    }
    
    @Override
    public String toString() {
        return localTeam + " - " + visitorTeam;
    }
}
