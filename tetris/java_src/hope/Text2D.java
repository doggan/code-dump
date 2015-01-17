package hope;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;

import javax.vecmath.Vector2f;

public class Text2D extends Primitive {
	Vector2f mPosition = new Vector2f(0, 0);
	Color mColor = Color.white;
	String mText = "";
	
	public Text2D() {

	}
	
	public Vector2f getPosition() {
		return mPosition;
	}
	public void setPosition(Vector2f position) {
		mPosition = position;
	}
	
	public void setPosition(Point position) {
		mPosition.set(position.x, position.y);
	}
	
	public Color getColor() {
		return mColor;
	}
	public void setColor(Color color) {
		mColor = color;
	}
	
	public String getText() {
		return mText;
	}
	public void setText(String text) {
		mText = text;
	}

	void draw(Graphics g) {
		g.setColor(mColor);
		g.drawString(getText(), (int)mPosition.x, (int)mPosition.y);
		
		
        // Set the font for example Lucida Sans Typewriter if it is
        // different from the default font
 //       String family = "Lucida Sans Typewriter";
  //      int style = Font.PLAIN;
   //     int size = 18;
    //    Font font = new Font(family, style, size);

//        g.setFont(font);
    
    /*
        // Draw a text such that the top-left corner is at x, y
        x = 100;
        y = 50;
        FontMetrics fontMetrics = g.getFontMetrics();
        g.drawString("text", x, y+fontMetrics.getAscent());
        */
	}
}
