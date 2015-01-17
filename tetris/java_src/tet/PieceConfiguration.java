package tet;

import hope.Sprite;

public class PieceConfiguration {
	/** Number of cells wide and high. */
	int mWidth, mHeight;
	
	int mPivotPointX = -1;
	int mPivotPointY = -1;
	
	private Sprite mCells[][];
	
	PieceConfiguration(int width, int height) {
		mWidth = width;
		mHeight = height;
		
		mCells = new Sprite[width][height];
	}
	
	// TODO: math util lib?
	private int clamp(int val, int min, int max) {
		if (val < min) {
			return min;
		}
		else if (val > max) {
			return max;
		}
		else {
			return val;
		}
	}
	
	/**
	 * Set the pivot point. Setting a negative value causes default rotation
	 * around the true center.
	 * @param x Horizontal pivot point.
	 * @param y Vertical pivot point.
	 */
	public void setPivotPoint(int x, int y) {
		mPivotPointX = clamp(x, -1, mWidth - 1);
		mPivotPointY = clamp(y, -1, mHeight - 1);
	}
	
	public void setCell(int x, int y, Sprite cell) {
		mCells[x][y] = cell;
	}
	
	/**
	 * Update all cell sprite positions to world (board) coordinates.
	 */
	public void setAbsoluteCellPosition(int x, int y, int cellWidth, int cellHeight) {
		for (int col = 0; col < mCells.length; col++) {
			for (int row = 0; row < mCells[col].length; row++) {
				Sprite cell = mCells[col][row];
				if (cell != null) {
					int absX = (x + col) * cellWidth;
					int absY = (y + row) * cellHeight;
					cell.setPosition(absX, absY);
				}
			}
		}
	}
}
