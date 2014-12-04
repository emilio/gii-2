import java.util.Properties;

interface IView <T> {
	public void render(T object, Properties params);
}
