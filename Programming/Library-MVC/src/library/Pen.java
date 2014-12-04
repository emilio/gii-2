/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package library;

import java.io.Serializable;
import java.util.Comparator;
import java.util.Random;

/**
 *
 * @author emilio
 */
public final class Pen implements IModel, Serializable {
	private final String manufacturer;
	private final String model;
	private final String color; // Easier for us to create if the user has to write it
	// private final Color color;
	private final float thickness;
	private float price;
	private int stock; // Stock is not final, since it may change

	private static final String[] MANUFACTURERS = {
		"BIC", "Faber Castell", "Rotring", "Flo-Master", "Flux pen",
		"Fountain pen", "Lewis Waterman", "Frost & Adams", "Fudepen",
		"Fisher", "Gel pen", "Joseph Gillott's (pens)"
	};

	/**
	 * Get a comparator for comparing different pens
	 *
	 * @param field the field name
	 * @return a valid comparator
	 *
	 * @throws java.lang.ClassNotFoundException
	 * @throws java.lang.InstantiationException
	 * @throws java.lang.IllegalAccessException
	 */
	public static Comparator getComparator(String field) throws ClassNotFoundException, InstantiationException, IllegalAccessException {
		return (Comparator) Class.forName("library.Pen" + field.substring(0, 1).toUpperCase() + field.substring(1).toLowerCase() + "Comparator").newInstance();
	}

	Pen() {
		Random r = new Random();
		manufacturer = Pen.MANUFACTURERS[r.nextInt(Pen.MANUFACTURERS.length - 1)];
		model = Integer.toString(r.nextInt(999)) + (char) ('a' + r.nextInt('z' - 'a'));
		color = new Color(r.nextInt(255), r.nextInt(255), r.nextInt(255)).toString();
		thickness = r.nextFloat() * 10; // Random float between 0 and 10
		price = r.nextFloat() * 20; // A 20 dollar pen is more than enough
		stock = r.nextInt(1000); // We won't buy more than a thousand
	}

	public Pen(String manufacturer, String model, String color, float price, float thickness, int stock) {
		this.manufacturer = manufacturer;
		this.model = model;
		this.color = color;
		this.thickness = thickness;
		this.price = price;
		this.stock = stock;
	}

	public String getManufacturer() {
		return manufacturer;
	}

	public String getModel() {
		return model;
	}

	public String getColor() {
		return color;
	}

	public float getThickness() {
		return thickness;
	}

	public float getPrice() {
		return price;
	}

	public void setPrice(float price) {
		this.price = price;
	}

	public int getStock() {
		return stock;
	}

	public void setStock(int stock) {
		this.stock = stock;
	}

	/**
	 * Check if current state is valid
	 */
	public boolean valid() {
		return !(manufacturer.length() == 0 ||
                        model.length() == 0 ||
                        stock < 0 ||
                        price < 0 ||
                        thickness <= 0);
	}
}
