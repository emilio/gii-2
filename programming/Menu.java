package matrixexample;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
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
		private final Method method;
		private final String description;


		public MenuOption(Method method, String description) {
			this.method = method;
			this.description = description;
		}

		public Method getMethod() {
			return method;
		}

		public String getDescription() {
			return description;
		}
	}

	private MenuOption chosenOption;
	private final Object target;
	private final ArrayList<MenuOption> options;
	private final String description;
	// If methods must be get from target or target.getClass();
	private final boolean isStatic;

	public Menu(String desc) {
		description = desc;
		target = this.getClass();
		isStatic = true;
		options = new ArrayList<MenuOption>();
	}

	public Menu( Class c, String desc) {
		description = desc;
		target = c;
		isStatic = true;
		options = new ArrayList<MenuOption>();
	}

	public Menu( Object o, String desc ) {
		description = desc;
		target = o;
		isStatic = false;
		options = new ArrayList<MenuOption>();
	}

	/**
	 * Get target class
	 */
	private Class getTargetClass() {
		return isStatic ? (Class) target : target.getClass();
	}

	/**
	 * Add an entry to the menu
	 *
	 * @param methodName the method name to invoke
	 * @param description the description used in render
	 */
	public final Menu addEntry(String methodName, String description) throws NoSuchMethodException {
		options.add(new MenuOption(getTargetClass().getMethod(methodName), description));
		return this;
	}

	/**
	 * Render menu
	 */
	public final Menu render() {
		int i = 0;
		System.out.println(description);

		for( MenuOption option : options ) {
			i++;
			System.out.println( i + ") " + option.getDescription());
		}

		return this;
	}

	public final Menu getResponse() {
		int response = 0;
		int max = options.size();
		Scanner sc = new Scanner(System.in);
		while ( ! (response > 0 && response <= max) ) {
			System.out.println("Introduce una opción: ");
			response = sc.nextInt();
		}

		chosenOption = options.get(response - 1);

		return this;
	}


	public final Object execute() throws IllegalAccessException, IllegalArgumentException, InvocationTargetException {
		return chosenOption.getMethod().invoke(target);
	}
}
