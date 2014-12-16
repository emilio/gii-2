/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvc;

import java.util.ArrayList;
import java.util.Scanner;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alumno
 */
public class Menu {
    private static final class MenuOption {
        private final String route;
        private final String description;
        private final Arguments arguments;

        public MenuOption(String route, Arguments args, String description) {
            this.route = route;
            this.description = description;
            this.arguments = args;
        }

        public MenuOption(String route, String description) {
            this.route = route;
            this.description = description;
            this.arguments = new Arguments();
        }

        public String getRoute() {
            return route;
        }

        public String getDescription() {
            return description;
        }

        public Arguments getArguments() {
            return arguments;
        }
    }

    private MenuOption chosenOption;
    private final Application app;
    private final ArrayList<MenuOption> options;
    private final String description;

    public Menu(String desc) {
        this.description = desc;
        this.app = Application.getMainInstance();
        this.options = new ArrayList<>();
    }

    public Menu( Application app, String desc) {
        this.description = desc;
        this.app = app;
        this.options = new ArrayList<>();
    }

    /**
     * Add an entry to the menu
     *
     * @param route
     * @param description the description used in render
     * 
     * @return self
     */
    public final Menu addEntry(String route, String description) {
        this.options.add(new MenuOption(route, description));
        return this;
    }

    public final Menu addEntry(String route, Arguments args, String description) {
        this.options.add(new MenuOption(route, args, description));
        return this;
    }

    /**
     * Render menu
     * 
     * @return self
     */
    public final Menu render() {
        int i = 0;
        System.out.println(description);

        for( MenuOption option : options ) {
                i++;
                System.out.println( i + ") " + option.getDescription());
        }
        
        System.out.println((i + 1) + ") Exit");

        return this;
    }

    public final Menu getResponse() {
        int response = 0;
        int max = options.size() + 1;
        Scanner sc = new Scanner(System.in);
        while ( ! (response > 0 && response <= max) ) {
                System.out.print("Write an option: ");
                response = sc.nextInt();
        }

        if ( response == max  )
            chosenOption = null;
        else
            chosenOption = options.get(response - 1);

        return this;
    }


    public final boolean execute() throws InvalidAppPathException, InvalidControllerException, InvalidActionException {
        if ( chosenOption == null )
            return false;
        app.execute(chosenOption.getRoute(), chosenOption.getArguments());
        return true;
    }
}
