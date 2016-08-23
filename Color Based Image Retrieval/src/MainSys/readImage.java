package MainSys;

import java.awt.image.BufferedImage;
import java.io.*;
import javax.imageio.ImageIO;

public class readImage {
	int imageCount = 1;
	double intensityBins[] = new double[25];
	int intensityMatrix[][] = new int[100][25];
	double colorCodeBins[] = new double[64];
	int colorCodeMatrix[][] = new int[100][64];

	/*
	 * Each image is retrieved from the file. The height and width are found for
	 * the image and the getIntensity and getColorCode methods are called.
	 */
	public readImage() {
		int[] rgb = new int[3];

		BufferedImage bi = null;
		while (imageCount < 101) {
			File file = new File("src/images/" + imageCount + ".jpg");
			try {
				// the line that reads the image file
				bi = ImageIO.read(file);
			} catch (IOException e) {
				System.out.println("Error occurred when reading the file.");
			}

			int width = bi.getWidth();
			int height = bi.getHeight();
			int minx = bi.getMinX();
			int miny = bi.getMinY();
			for (int i = minx; i < width; i++)
				for (int j = miny; j < height; j++) {
					int pixel = bi.getRGB(i, j);
					rgb[0] = (pixel & 0xff0000) >> 16;
					rgb[1] = (pixel & 0xff00) >> 8;
					rgb[2] = (pixel & 0xff);
					getIntensity(rgb, imageCount);
					getColorCode(rgb, imageCount);
				}
			imageCount++;
		}

		writeColorCode();
		writeIntensity();
	}

	// intensity method

	public void getIntensity(int[] rgb, int imageCount) {

		double isty = 0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2];
		if (isty >= 240) {
			intensityMatrix[imageCount - 1][24]++;
		} else {
			int index = (int) (isty / 10);
			intensityMatrix[imageCount - 1][index]++;
		}
	}

	// color code method
	public void getColorCode(int[] rgb, int imageCount) {

		rgb[0] = (rgb[0] & 0xC0) >> 2;
		rgb[1] = (rgb[1] & 0xC0) >> 4;
		rgb[2] = (rgb[2] & 0xC0) >> 6;
		int index = rgb[0] + rgb[1] + rgb[2];
		colorCodeMatrix[imageCount - 1][index]++;

	}

	// This method writes the contents of the colorCode matrix to a file named
	// colorCodes.txt.
	public void writeColorCode() {
		BufferedWriter bufferWritter;
		File file = new File("colorCodes.txt");
		// if file exists, clear all the contents
		if (file.exists()){
			try {
				FileWriter fileWritter = new FileWriter(file.getName(), false);
				bufferWritter = new BufferedWriter(
						fileWritter);
				bufferWritter.write("");
				bufferWritter.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		// if file does not exist, create one
		else{
			try {
				file.createNewFile();
			}
			catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		try {
			FileWriter fileWritter = new FileWriter(file.getName(),
					true);
			bufferWritter = new BufferedWriter(
					fileWritter);
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 64; j++) {
					bufferWritter.write(colorCodeMatrix[i][j] + " ");
				}
				bufferWritter.write("\n");
			}
			bufferWritter.close();
		}catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("Color code done");
	}

	// This method writes the contents of the intensity matrix to a file called
	// intensity.txt
	public void writeIntensity() {
		BufferedWriter bufferWritter;
		File file = new File("intensity.txt");
		// if file exists, clear all the contents
		if (file.exists()){
			try {
				FileWriter fileWritter = new FileWriter(file.getName(), false);
				bufferWritter = new BufferedWriter(
						fileWritter);
				bufferWritter.write("");
				bufferWritter.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		// if file does not exist, create one
		else{
			try {
				file.createNewFile();
			}
			catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		try {
			FileWriter fileWritter = new FileWriter(file.getName(),
					true);
			bufferWritter = new BufferedWriter(
					fileWritter);
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 25; j++) {
					bufferWritter.write(intensityMatrix[i][j] + " ");
				}
				bufferWritter.write("\n");
			}
			bufferWritter.close();
		}catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("Intensity done");
	}

	public static void main(String[] args) {
		new readImage();
	}

}
