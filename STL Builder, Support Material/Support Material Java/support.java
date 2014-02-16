package supportFile;

import java.io.*;
import java.util.*;

public class support {

	public static void main (String [] args) {
		generate("sphere.stl", "SS.stl", 1, 0);
	}

	public static void generate (String fileName, String fileTo, int buffer, int height) {
		double xMin = Double.POSITIVE_INFINITY;
		double yMin = Double.POSITIVE_INFINITY;
		double xMax = Double.NEGATIVE_INFINITY;
		double yMax = Double.NEGATIVE_INFINITY;
		double zMax = Double.NEGATIVE_INFINITY;
		double temp;
		String tmp;
		try {
			BufferedReader file = new BufferedReader(new FileReader(fileName));
			PrintWriter fileOut = new PrintWriter (new FileOutputStream (fileTo,false));
			try {
				while(file.ready()) {
					String line = file.readLine();
					StringTokenizer s = new StringTokenizer(line);
					tmp = s.nextToken().toLowerCase();
					if (tmp.equals("facet")) {
						s.nextToken();
						fileOut.print("\tfacet normal ");
						for (int i = 0; i < 3; ++i) {
							fileOut.printf("%7e ", (Double.parseDouble(s.nextToken()) * -1));
						}
						fileOut.println("");
					} else if (tmp.equals("vertex")) {
						temp = Double.parseDouble(s.nextToken());
						if (xMin > temp) {
							xMin = temp;
						} else if (xMax < temp) {
							xMax = temp;
						}
						temp = Double.parseDouble(s.nextToken());
						if (yMin > temp) {
							yMin = temp;
						} else if (yMax < temp) {
							yMax = temp;
						}
						temp = Double.parseDouble(s.nextToken());
						if (zMax < temp) {
							zMax = temp;
						}
						fileOut.println(line);
					} else if (tmp.equals("endsolid") == false){
						fileOut.println(line);
					}
				}//end file ready			
			} catch (IOException l) {
				System.out.println(l.getMessage());
			}
			fileOut.close();
			xMin = xMin - buffer; xMax = xMax + buffer;
			yMin = yMin - buffer; yMax = yMax + buffer;
			zMax = zMax - height;
			generateWall(fileTo, xMin, xMax, yMin, yMax, zMax);
		} catch(FileNotFoundException e) {
			System.out.println(e.getMessage());
		}
	}

	public static void generateWall (String fileName, double xMin, double xMax, double yMin, double yMax, double zMax) {
		double [] p1 = new double [3], p2 = new double [3], p3 = new double [3], p4 = new double [3];
		double [] p5 = new double [3], p6 = new double [3], p7 = new double [3], p8 = new double [3];
		//Xmin, yMin, 0 point
		p1[0] = xMin; p1[1] = yMin; p1[2] = 0;			//0
		p2[0] = xMax; p2[1] = yMin; p2[2] = 0;			//1
		p3[0] = xMin; p3[1] = yMin; p3[2] = zMax;
		p4[0] = xMax; p4[1] = yMin; p4[2] = zMax;
		p5[0] = xMin; p5[1] = yMax; p5[2] = 0;
		p6[0] = xMax; p6[1] = yMax; p6[2] = 0;
		p7[0] = xMin; p7[1] = yMax; p7[2] = zMax;
		p8[0] = xMax; p8[1] = yMax; p8[2] = zMax;
		writeTriangle (fileName, p1, p2, p3, p4);
		writeTriangle (fileName, p2, p6, p4, p8);
		//writeTriangle (fileName, p1, p5, p2, p6); //Base 
		writeTriangle(fileName, p1, p3, p5, p7);
		writeTriangle (fileName, p5, p7, p6, p8);
		try {
		PrintWriter file = new PrintWriter(new FileOutputStream(fileName, true));
		file.println("endsolid");
		file.close();
		} catch (Exception e) {
		};
	}
	
	/**Creates 2 triangles given 4 points, p1, p2 are on the bottom, p3 p3 are on top, the square is defined by 
	 * p1, p3, p4, p2
	 */
	public static void writeTriangle (String fileName, double[] p1, double[] p2, double[] p3, double[] p4) {
		double [] v1 = new double [3],v2 = new double [3],n = new double [3];			
			v1[0] = p2[0] - p1[0];
			v1[1] = p2[1] - p1[1];
			v1[2] = p2[2] - p1[2];
			v2[0] = p3[0] - p1[0];
			v2[1] = p3[1] - p1[1];
			v2[2] = p3[2] - p1[2];
 			n[0] = v1[1]*v2[2]-v1[2]*v2[1];
			n[1] = -(v1[0]*v2[2] - v1[2]*v2[0]);
			n[2] = v1[0]*v2[1] - v1[1]*v2[0];
			double nMag = Math.sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
			n[0] = n[0]/ nMag;
			n[1] = n[1]/nMag;
			n[2] = n[2]/nMag;
			try {
			PrintWriter fileOut = new PrintWriter (new FileOutputStream (fileName,true));
			//Triangle 1
			fileOut.printf ("\tfacet normal %7.6e %7.6e %7.6e\n", n[0], n[1], n[2]);
			fileOut.printf("\t\touter loop\n");
			fileOut.printf("\t\t\tvertex %7.6e %7.6e %7.6e\n", p1[0], p1[1], p1[2]);
			fileOut.printf("\t\t\tvertex %7.6e %7.6e %7.6e\n", p2[0], p2[1], p2[2]);
			fileOut.printf("\t\t\tvertex %7.6e %7.6e %7.6e\n", p3[0], p3[1], p3[2]);
			fileOut.printf("\t\tendloop\n");
			fileOut.printf("\tendfacet\n");
			//Triangle 2
			fileOut.printf ("\tfacet normal %7.6e %7.6e %7.6e\n", n[0], n[1], n[2]);
			fileOut.printf("\t\touter loop\n");
			fileOut.printf("\t\t\tvertex %7.6e %7.6e %7.6e\n", p4[0], p4[1], p4[2]);
			fileOut.printf("\t\t\tvertex %7.6e %7.6e %7.6e\n", p2[0], p2[1], p2[2]);
			fileOut.printf("\t\t\tvertex %7.6e %7.6e %7.6e\n", p3[0], p3[1], p3[2]);
			fileOut.printf("\t\tendloop\n");
			fileOut.printf("\tendfacet\n");
			fileOut.close();
		} catch (FileNotFoundException e) {
			System.err.println(e.getMessage());
		}
	}

}


