package MainSys;

import java.awt.BorderLayout;
import java.awt.EventQueue;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.BoxLayout;

import java.awt.GridLayout;

import javax.swing.JLabel;
import javax.swing.JButton;

import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.ColumnSpec;
import com.jgoodies.forms.factories.FormFactory;
import com.jgoodies.forms.layout.RowSpec;

import javax.swing.border.LineBorder;

import java.awt.Color;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Scanner;
import java.util.StringTokenizer;
import java.util.TreeMap;

import javax.swing.JRadioButton;

import java.awt.Dimension;
import java.awt.FlowLayout;

import javax.swing.SwingConstants;
import javax.swing.border.TitledBorder;
import javax.swing.JToggleButton;
import javax.swing.JCheckBox;

public class CBIR extends JFrame {

	private JPanel contentPane; // root panel
	private JPanel displayPanel; // browse images in database
	private JLabel findphoto; // display the image a user selects
	private JLabel PageLabel; // display the page number
	private JButton[] button; // creates an array of JButtons
	private JToggleButton[] toggleButton; // for selecting relevant images
	private JPanel[] containerPanel; // contains each image in the database
	private JRadioButton radioIntensity; // for selecting intensity method
	private JRadioButton radioColorCode; // for selecting colorcode method
	private JRadioButton radioIntensityColor; // for doing Relevance Feedback retrieval
	private JCheckBox relevanceCkButton; // to enable selecting relevant images

	private int[] buttonOrder = new int[101]; // creates an array to keep up with the image order
	private double[] imageSize = new double[100]; // keeps up with the image sizes
	private Integer[][] intensityMatrix = new Integer[100][25];
	private Integer[][] colorCodeMatrix = new Integer[100][64];
	private double featureMatrix[][] = new double[100][89]; // store the features for each image
	private Map<Integer, TreeMap<Double, Integer>> map; // store each image's retrieval result

	int picNo = 0; // the number of image a user selects
	int imageCount = 1; // keeps up with the number of images displayed since the first page.
	int pageNo = 1;
	boolean isInIteration = false; // to see if in the RF iteration
	private ArrayList<Integer> relevantImage; // store relevant images	
	double standardDeviation[]; // the standard deviation of each feature
	double updatedWeight[]; // updated weight for each feature
	double normalizedWeight[]; 	// normalized weight for each feature

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					CBIR frame = new CBIR();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public CBIR() {

		// set root panel and layout
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		contentPane.setLayout(new BorderLayout(0, 0));

		// set display panel
		displayPanel = new JPanel();
		displayPanel.setLayout(new GridLayout(4, 5, 5, 5));

		// set menu panel for operation
		JPanel menuPanel = new JPanel();
		menuPanel.setPreferredSize(new Dimension(290, 10));
		menuPanel.setBorder(new LineBorder(new Color(0, 0, 0)));
		menuPanel.setLayout(new BorderLayout(0, 0));

		// set view panel for display
		JPanel viewPanel = new JPanel();
		viewPanel.setBorder(new EmptyBorder(0, 5, 0, 0));
		viewPanel.setPreferredSize(new Dimension(750, 10));
		viewPanel.setMaximumSize(new Dimension(750, 750));
		viewPanel.setLayout(new BorderLayout(0, 0));

		// set photo panel
		JPanel photoPanel = new JPanel();
		photoPanel.setPreferredSize(new Dimension(280, 280));
		photoPanel.setLayout(new GridLayout(0, 1, 0, 0));

		// set panel for radio buttons
		JPanel operationPanel = new JPanel();
		operationPanel.setPreferredSize(new Dimension(280, 10));

		// set page switch panel
		JPanel pageSwitchPanel = new JPanel();

		// set panel contains different methods
		JPanel methodPanel = new JPanel();
		methodPanel.setPreferredSize(new Dimension(280, 150));
		methodPanel.setBorder(new TitledBorder(null, "Method",
				TitledBorder.LEADING, TitledBorder.TOP, null, null));
		methodPanel.setLayout(new BorderLayout(0, 0));

		// set buttonPanel
		JPanel buttonPanel = new JPanel();
		buttonPanel.setPreferredSize(new Dimension(280, 100));
		buttonPanel.setBorder(new TitledBorder(null, "Operation",
				TitledBorder.LEADING, TitledBorder.TOP, null, null));
		buttonPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 5));

		JButton previousPage = new JButton("Previous Page");
		previousPage.addActionListener(new previousPageHandler());

		JButton nextPage = new JButton("Next Page");
		nextPage.addActionListener(new nextPageHandler());

		// set search button
		JButton searchButton = new JButton("Search");
		searchButton.addActionListener(new searchButtonHandler());

		// set reset button
		JButton reset = new JButton("Reset");
		reset.addActionListener(new resetButtonHandler());

		// three radio buttons for selecting methods, default selection is intensity
		radioIntensity = new JRadioButton("Intensity");
		radioIntensity.setSelected(true);
		radioIntensity.addActionListener(new radioButtonHandler());

		radioColorCode = new JRadioButton("Color Code");
		radioColorCode.addActionListener(new radioButtonHandler());

		radioIntensityColor = new JRadioButton("Intensity & Color Code");
		radioIntensityColor.addActionListener(new radioButtonHandler());

		// set relevance check box
		relevanceCkButton = new JCheckBox("Relevance");
		relevanceCkButton.setEnabled(false);
		relevanceCkButton.addActionListener(new relevanceButtonHandler());

		// set find photo label
		findphoto = new JLabel("Find Photo");
		findphoto.setPreferredSize(new Dimension(280, 280));
		findphoto.setBorder(new LineBorder(new Color(0, 0, 0)));
		findphoto.setVerticalTextPosition(JLabel.BOTTOM);
		findphoto.setHorizontalTextPosition(JLabel.CENTER);
		findphoto.setHorizontalAlignment(JLabel.CENTER);

		// set page label
		PageLabel = new JLabel("1/5");

		// group the radio buttons
		ButtonGroup methodGroup = new ButtonGroup();
		operationPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));

		// add each element
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("Icon Demo: Please Select an Image");
		setSize(1300, 750);
		setContentPane(contentPane);
		contentPane.add(menuPanel, BorderLayout.WEST);
		contentPane.add(viewPanel);
		menuPanel.add(photoPanel, BorderLayout.NORTH);
		menuPanel.add(operationPanel);
		viewPanel.add(displayPanel, BorderLayout.CENTER);
		viewPanel.add(pageSwitchPanel, BorderLayout.NORTH);
		photoPanel.add(findphoto);
		operationPanel.add(methodPanel);
		operationPanel.add(buttonPanel);
		pageSwitchPanel.add(previousPage);
		pageSwitchPanel.add(PageLabel);
		pageSwitchPanel.add(nextPage);
		methodPanel.add(radioIntensity, BorderLayout.NORTH);
		methodPanel.add(radioColorCode, BorderLayout.CENTER);
		methodPanel.add(radioIntensityColor, BorderLayout.SOUTH);
		buttonPanel.add(searchButton);
		buttonPanel.add(relevanceCkButton);
		buttonPanel.add(reset);
		methodGroup.add(radioIntensity);
		methodGroup.add(radioColorCode);
		methodGroup.add(radioIntensityColor);

		containerPanel = new JPanel[101];
		button = new JButton[101];
		toggleButton = new JToggleButton[101];

		/*
		 * This for loop goes through the images in the database and stores them
		 * as icons and adds the images to JButtons and then to the JButton
		 * array
		 */
		for (int i = 1; i < 101; i++) {
			ImageIcon icon;
			icon = new ImageIcon(getClass()
					.getResource("/images/" + i + ".jpg"));
			if (icon != null) {
				imageSize[i - 1] = icon.getIconWidth() * icon.getIconHeight();
				icon.setImage(icon.getImage().getScaledInstance(200, 140,
						Image.SCALE_DEFAULT));
				containerPanel[i] = new JPanel();
				containerPanel[i].setBorder(new LineBorder(new Color(0, 0, 0)));
				containerPanel[i].setLayout(new BorderLayout(0, 0));
				button[i] = new JButton(icon);
				button[i].setBorder(new EmptyBorder(0, 0, 0, 0));
				toggleButton[i] = new JToggleButton("Relevant");
				toggleButton[i].setVisible(false);
				button[i].addActionListener(new IconButtonHandler(i));
				toggleButton[i].addActionListener(new ToggleButtonHandler(i));
				buttonOrder[i] = i;
			}
		}

		readIntensityFile();
		readColorCodeFile();
		computeFeatures();
		displayFirstPage();
	}

	/*
	 * This method opens the intensity text file containing the intensity matrix
	 * with the histogram bin values for each image. The contents of the matrix
	 * are processed and stored in a two dimensional array called
	 * intensityMatrix.
	 */
	public void readIntensityFile() {
		Scanner read;
		try {
			read = new Scanner(new File("intensity.txt"));
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 25; j++) {
					intensityMatrix[i][j] = read.nextInt();
					// set the first 25 features
					featureMatrix[i][j] = intensityMatrix[i][j] / imageSize[i];
				}
			}
		} catch (FileNotFoundException EE) {
			System.out.println("The file intensity.txt does not exist");
		}
	}

	/*
	 * This method opens the color code text file containing the color code
	 * matrix with the histogram bin values for each image. The contents of the
	 * matrix are processed and stored in a two dimensional array called
	 * colorCodeMatrix.
	 */
	private void readColorCodeFile() {
		Scanner read;
		try {
			read = new Scanner(new File("colorCodes.txt"));
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 64; j++) {
					colorCodeMatrix[i][j] = read.nextInt();
					// set the last 64 features
					featureMatrix[i][j + 25] = colorCodeMatrix[i][j]
							/ imageSize[i];
				}
			}
		} catch (FileNotFoundException EE) {
			System.out.println("The file intensity.txt does not exist");
		}

	}

	/*
	 * This method is to normalize each image's features
	 */
	private void computeFeatures() {

		double sum[] = new double[89];
		double avg[] = new double[89];
		double sd[] = new double[89];
		double dev[] = new double[89];

		// compute the average of each feature
		for (int i = 0; i < 89; i++) {
			for (int j = 0; j < 100; j++) {
				sum[i] += featureMatrix[j][i];
			}
			avg[i] = sum[i] / 100;
		}

		// compute standard deviation of each feature
		for (int i = 0; i < 89; i++) {
			for (int j = 0; j < 100; j++) {
				dev[i] += Math.pow(featureMatrix[j][i] - avg[i], 2);
			}
			sd[i] = Math.sqrt(dev[i] / 99);
		}

		// normalize each image's features
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 89; j++) {
				if (sd[j] != 0)
					featureMatrix[i][j] = (featureMatrix[i][j] - avg[j])
							/ sd[j];
				else
					featureMatrix[i][j] = 0;
			}
		}
	}

	/*
	 * This method displays the first twenty images in the panelBottom. The for
	 * loop starts at number one and gets the image number stored in the
	 * buttonOrder array and assigns the value to imageButNo. The button
	 * associated with the image is then added to panelBottom1. The for loop
	 * continues this process until twenty images are displayed in the
	 * panelBottom1
	 */
	private void displayFirstPage() {
		int imageButNo = 0;
		displayPanel.removeAll();
		for (int i = 1; i < 21; i++) {
			imageButNo = buttonOrder[i];
			// show each image in the display panel
			containerPanel[i].removeAll();
			containerPanel[i].add(button[imageButNo], BorderLayout.NORTH);
			containerPanel[i].add(toggleButton[imageButNo], BorderLayout.CENTER);
			containerPanel[i].revalidate();
			containerPanel[i].repaint();
			imageCount++;
			displayPanel.add(containerPanel[i]);
		}
		displayPanel.revalidate();
		displayPanel.repaint();

	}

	/*
	 * This method to update each feature's weight for computing distance.
	 */
	private void computeUpdatedWeight() {

		double sum[] = new double[89];
		double avg[] = new double[89];
		double variance[] = new double[89];
		double minSD;

		// compute sum of weights
		double sumUpdatedWeight = 0;

		
		// compute the sum of each feature
		for (int i = 0; i < relevantImage.size(); i++) {
			for (int j = 0; j < 89; j++) {
				sum[j] += featureMatrix[relevantImage.get(i) - 1][j];
			}
		}

		// compute the average of each feature
		for (int i = 0; i < 89; i++) {
			avg[i] = sum[i] / relevantImage.size();
		}

		// compute variance of each feature
		for (int i = 0; i < relevantImage.size(); i++) {
			for (int j = 0; j < 89; j++) {
				variance[j] += Math.pow(
						(featureMatrix[relevantImage.get(i) - 1][j] - avg[j]),
						2);
			}
		}

		minSD = 1000000;
		
		// compute standard deviation and minimal non-zero standard deviation
		for (int i = 0; i < 89; i++) {
			standardDeviation[i] = Math.sqrt(variance[i] / (relevantImage.size() - 1 ) );
			if (standardDeviation[i] < minSD && standardDeviation[i] > 0) {
				minSD = standardDeviation[i];
			}
		}

		// compute sum of updated weights
		for (int i = 0; i < 89; i++) {
			if (standardDeviation[i] <= 0.0) {
				if (avg[i] == 0)
					updatedWeight[i] = 0;
				else
					updatedWeight[i] = 1 / (0.5 * minSD);
			} else {
				updatedWeight[i] = 1 / standardDeviation[i];
			}
			sumUpdatedWeight += updatedWeight[i];
		}

		// compute normalized weights for each feature
		for (int i = 0; i < 89; i++) {
			normalizedWeight[i] = updatedWeight[i] / sumUpdatedWeight;
		}
	}

	/*
	 * This class implements an ActionListener for search button. When the search
	 * button is clicked, system will retrieve images based on selected method.
	 */
	private class searchButtonHandler implements ActionListener {

		public void actionPerformed(ActionEvent e) {

			// if no image is selected
			if (picNo == 0)
				new Warning("select an image first");

			else {
				double[] distance = new double[100]; // stores each image's distance from the selected image
				TreeMap<Double, Integer> treemap = new TreeMap<Double, Integer>(); 	// a tree map is for sorting distance values automatically
				map = new HashMap<Integer, TreeMap<Double, Integer>>();
				int pic = (picNo - 1);

				// display retrieval results based on the selected radio button
				// if intensity is selected
				if (radioIntensity.isSelected()) {
					isInIteration = false;
					relevanceCkButton.setSelected(false);
					
					// set all toggle button not enable
					for (int i = 1; i < 101; i++) {
						buttonOrder[i] = i;
						toggleButton[i].setSelected(false);
					}
					
					// compute the distance values and put them in the tree map
					for (int i = 0; i < 100; i++) {
						for (int j = 0; j < 25; j++) {
							distance[i] += Math.abs(intensityMatrix[i][j]
									/ imageSize[i] - intensityMatrix[pic][j]
									/ imageSize[pic]);
						}
						treemap.put(distance[i], i);
					}
					map.put(pic, treemap);

					// store each image's order ranked by distance
					Iterator<Double> it = treemap.keySet().iterator();
					int count = 1;
					while (it.hasNext()) {
						double dis = it.next();
						buttonOrder[count] = treemap.get(dis) + 1;
						count++;
					}
				}
				// if color code is selected
				else if (radioColorCode.isSelected()) {
					isInIteration = false;
					relevanceCkButton.setSelected(false);
					
					// set all toggle button not enable
					for (int i = 1; i < 101; i++) {
						buttonOrder[i] = i;
						toggleButton[i].setSelected(false);
					}
					
					// compute the distance values and put them in the tree map
					for (int i = 0; i < 100; i++) {
						for (int j = 0; j < 64; j++) {
							distance[i] += Math.abs(colorCodeMatrix[i][j]
									/ imageSize[i] - colorCodeMatrix[pic][j]
									/ imageSize[pic]);
						}
						treemap.put(distance[i], i);
					}
					map.put(pic, treemap);

					// store each image's order ranked by distance
					Iterator<Double> it = treemap.keySet().iterator();
					int count = 1;
					while (it.hasNext()) {
						double dis = it.next();
						buttonOrder[count] = treemap.get(dis) + 1;
						count++;
					}
				}
				// if intensity & color code is selected
				else {
					// if it's not in the iteration
					if (!isInIteration) {
						relevantImage = new ArrayList<Integer>(100);
						
						// compute the distance values and put them in the tree map
						for (int i = 0; i < 100; i++) {
							for (int j = 0; j < 89; j++) {
								distance[i] += Math.abs(featureMatrix[i][j]
										- featureMatrix[pic][j]);
							}
							treemap.put(distance[i], i);
						}
						map.put(pic, treemap);
						
						// store each image's order ranked by distance
						Iterator<Double> it = treemap.keySet().iterator();
						int count = 1;
						while (it.hasNext()) {
							double dis = it.next();
							buttonOrder[count] = treemap.get(dis) + 1;
							count++;
						}
						isInIteration = true;
						relevanceCkButton.setEnabled(true);
					} else {
						// if the relevance check box is not selected, select it automatically and inform user
						if (!relevanceCkButton.isSelected()) {
							new Warning(
									"Relevance is automatically selected for you!");
							relevanceCkButton.setSelected(true);
							relevanceButtonHandler rbh = new relevanceButtonHandler();
							rbh.actionPerformed(null);
						} else {
							standardDeviation = new double[89];
							updatedWeight = new double[89];
							normalizedWeight = new double[89];
							
							// if no relevant images are selected
							if (relevantImage.isEmpty()) {
								for (int i = 0; i < 89; i++) {
									normalizedWeight[i] = 1;
								}
							} else
								computeUpdatedWeight();

							// compute the distance values and put them in the tree map
							for (int i = 0; i < 100; i++) {
								for (int j = 0; j < 89; j++) {
									distance[i] += normalizedWeight[j]
											* Math.abs(featureMatrix[i][j]
													- featureMatrix[pic][j]);
								}
								treemap.put(distance[i], i);
							}
							map.put(pic, treemap);
							
							// store each image's order ranked by distance
							Iterator<Double> it = treemap.keySet().iterator();
							int count = 1;
							while (it.hasNext()) {
								double dis = it.next();
								buttonOrder[count] = treemap.get(dis) + 1;
								count++;
							}
						}
					}
				}
				imageCount = 1;
				PageLabel.setText("1/5");
				displayFirstPage();
			}
		}
	}

	/*
	 * This class implements an ActionListener for relevance check box. When
	 * check box is selected, show all toggle buttons.
	 */
	private class relevanceButtonHandler implements ActionListener {

		public void actionPerformed(ActionEvent e) {
			if (relevanceCkButton.isSelected()) {
				for (int i = 1; i < 101; i++) {
					toggleButton[i].setVisible(true);
				}
			} else {
				for (int i = 1; i < 101; i++) {
					toggleButton[i].setVisible(false);
				}
			}
		}

	}

	/*
	 * This class implements an ActionListener for each radio button. When an radio
	 * button is clicked, change some setting.
	 */
	private class radioButtonHandler implements ActionListener {

		public void actionPerformed(ActionEvent arg0) {
			if (radioIntensityColor.isSelected()) {
				if (isInIteration) {
					relevanceCkButton.setEnabled(true);
					relevanceButtonHandler rbh = new relevanceButtonHandler();
					rbh.actionPerformed(null);
				}
			}
			else {
				for (int i = 1; i < 101; i++) {
					toggleButton[i].setVisible(false);
				}
				relevanceCkButton.setEnabled(false);
			}
		}

	}

	/*
	 * This class implements an ActionListener for reset button. When
	 * button is clicked, reset all setting.
	 */
	private class resetButtonHandler implements ActionListener {

		public void actionPerformed(ActionEvent arg0) {
			for (int i = 1; i < 101; i++) {
				buttonOrder[i] = i;
				toggleButton[i].setVisible(false);
				toggleButton[i].setSelected(false);
			}
			relevanceCkButton.setSelected(false);
			relevanceCkButton.setEnabled(false);
			picNo = 0;
			findphoto.setIcon(null);
			isInIteration = false;
			displayFirstPage();
		}

	}

	/*
	 * This class implements an ActionListener for each iconButton. When an icon
	 * button is clicked, the image on the the button is added to the
	 * photographLabel and the picNo is set to the image number selected and
	 * being displayed.
	 */
	private class IconButtonHandler implements ActionListener {
		int pNo = 0;
		ImageIcon iconUsed;

		IconButtonHandler(int i) {
			pNo = i;
			iconUsed = new ImageIcon(getClass().getResource(
					"/images/" + i + ".jpg")); // sets the icon to the one used
												// in the button
		}

		public void actionPerformed(ActionEvent e) {
			iconUsed.setImage(iconUsed.getImage().getScaledInstance(290, 300,
					Image.SCALE_DEFAULT));
			findphoto.setIcon(iconUsed);
			picNo = pNo;
			for (int i = 1; i < 101; i++) {
				buttonOrder[i] = i;
				toggleButton[i].setVisible(false);
				toggleButton[i].setSelected(false);
			}
			relevanceCkButton.setSelected(false);
			relevanceCkButton.setEnabled(false);
			isInIteration = false;

		}

	}

	/*
	 * This class implements an ActionListener for each toggle button. When an toggle
	 * button is clicked, the corresponding image number is stored into relevantImage
	 */
	private class ToggleButtonHandler implements ActionListener {
		int pNo = 0;

		ToggleButtonHandler(int i) {
			pNo = i;
		}

		public void actionPerformed(ActionEvent e) {
			if (toggleButton[pNo].isSelected()) {
				relevantImage.add(pNo);
			} else {
				Integer in = new Integer(pNo);
				relevantImage.remove(in);
			}
		}

	}

	/*
	 * This class implements an ActionListener for the nextPageButton. The last
	 * image number to be displayed is set to the current image count plus 20.
	 * If the endImage number equals 101, then the next page button does not
	 * display any new images because there are only 100 images to be displayed.
	 * The first picture on the next page is the image located in the
	 * buttonOrder array at the imageCount
	 */
	private class nextPageHandler implements ActionListener {

		public void actionPerformed(ActionEvent e) {
			int imageButNo = 0;
			int endImage = imageCount + 20;
			if (endImage <= 101) {
				displayPanel.removeAll();
				for (int i = imageCount; i < endImage; i++) {
					imageButNo = buttonOrder[i];
					containerPanel[i].removeAll();
					containerPanel[i].add(button[imageButNo],
							BorderLayout.CENTER);
					containerPanel[i].add(toggleButton[imageButNo],
							BorderLayout.SOUTH);
					containerPanel[i].revalidate();
					containerPanel[i].repaint();
					imageCount++;
					displayPanel.add(containerPanel[i]);
				}
				PageLabel.setText(endImage / 20 + "/5"); // set the label which
															// indicate which
															// page is being
															// displayed
				displayPanel.revalidate();
				displayPanel.repaint();
			}
			else
				new Warning("This is the last page!");
		}

	}

	/*
	 * This class implements an ActionListener for the previousPageButton. The
	 * last image number to be displayed is set to the current image count minus
	 * 40. If the endImage number is less than 1, then the previous page button
	 * does not display any new images because the starting image is 1. The
	 * first picture on the next page is the image located in the buttonOrder
	 * array at the imageCount
	 */
	private class previousPageHandler implements ActionListener {

		public void actionPerformed(ActionEvent e) {
			int imageButNo = 0;
			int startImage = imageCount - 40;
			int endImage = imageCount - 20;
			if (startImage >= 1) {
				displayPanel.removeAll();
				/*
				 * The for loop goes through the buttonOrder array starting with
				 * the startImage value and retrieves the image at that place
				 * and then adds the button to the panelBottom1.
				 */
				for (int i = startImage; i < endImage; i++) {
					imageButNo = buttonOrder[i];
					containerPanel[i].removeAll();
					containerPanel[i].add(button[imageButNo],
							BorderLayout.CENTER);
					containerPanel[i].add(toggleButton[imageButNo],
							BorderLayout.SOUTH);
					containerPanel[i].revalidate();
					containerPanel[i].repaint();
					imageCount--;
					displayPanel.add(containerPanel[i]);

				}
				// set the label which indicate which page is being displayed
				PageLabel.setText(endImage / 20 + "/5");
				displayPanel.revalidate();
				displayPanel.repaint();
			}
			else
				new Warning("This is the first page!");
		}

	}
}
