/* Display Routine */

/* Ryan McCoppin 's project 1
 * September 14, 2012
 * Computer Vision, CEG 7550
 * Due: October 11, 2012 @ 3:30pm

 */

#include "define.h"

ByteImage bimg, second;				//A copy of the image to be viewed
int width, height, k, num_points;	//Window dimensions
double sigmad=2, sigmar=10, sigma = 3, threshold=0;
std::vector<point> points;
bool usingRegistration = false;
char option;

GLfloat zoomx = 1.0, zoomy = 1.0;	//Pixel zoom
int win;							//Window index
int win2;

void resetViewer();

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	if ((w!=width) || (h!=height)) {
		zoomx=(GLfloat)w/(GLfloat)bimg.nc;
		zoomy=(GLfloat)h/(GLfloat)bimg.nr;
		glPixelZoom(zoomx,zoomy);
	}
	width=w; height=h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
	glutPostRedisplay();
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) &&
		(zoomx==1.0) && (zoomy==1.0)){
			printf(" row=%d, col=%d, int=%d.\n", y,x, (int)bimg.image[y*bimg.nc+x]);
			if (usingRegistration)
			{
				point p;
				p.x=x; p.y=y; p.I=(int)bimg.image[y*bimg.nc+x];
				points.push_back(p);
				if (points.size() >= num_points) 
				{
					usingRegistration = false;
					registration(bimg, second, points, k);
					points.clear();
					//glutDestroyWindow(win);
					//resetViewer();
				}
			}
			glutPostRedisplay();
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);         
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glDrawPixels((GLsizei)bimg.nc,(GLsizei)bimg.nr, GL_LUMINANCE, GL_UNSIGNED_BYTE, bimg.image);
	glutSwapBuffers();
}

void initg() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	bimg.flip();
	width=bimg.nc; height=bimg.nr;
}



// Called by resetViewer() method
void menu(int id) {
	glutSwapBuffers();

	switch (id){
		case 1://Open image
			bimg = openImageDialog();
			glutDestroyWindow(win);
			resetViewer();
			break;	

		case 2://Save image
			bimg.flip();
			saveImageDialog(bimg);
			bimg.flip();
			break;

		case 3: // Call bilateral filtering on image and display
			std::cout << "*/*\\* Welcome to the Bilateral Filter */*\\*" << std::endl;
			bimg = openImageDialog();
			std::cout << "Please enter sigma-d: "; std::cin >> sigmad;
			std::cout << std::endl;

			std::cout << "Please enter sigma-r: "; std::cin >> sigmar;
			std::cout << std::endl;


			bilateral_filter(bimg,sigmad,sigmar);
			glutDestroyWindow(win);
			resetViewer();
			break;
		case 4: // Call gaussian smoothing
			std::cout << "*/*\\* Welcome to the Gaussian Filter */*\\*" << std::endl;
			bimg = openImageDialog();
			std::cout << "Please enter sigma: "; std::cin >> sigma;
			std::cout << std::endl;


			gaussian_filter(bimg,sigma);
			glutDestroyWindow(win);
			resetViewer();
			break;
		case 5: // Call edge detector
			std::cout << "*/*\\* Welcome to the Edge Detector! */*\\*" << std::endl;
			bimg = openImageDialog();
			std::cout << "Please enter sigma: "; std::cin >> sigma;
			std::cout << std::endl;
			edge_detector(bimg,sigma);
			glutDestroyWindow(win);
			resetViewer();
			break;
		case 6: // Call edge gradient detectors
			std::cout << "*/*\\* Welcome to the Edge Gradients Viewer! */*\\*" << std::endl;
			bimg = openImageDialog();
			std::cout << "Please enter sigma: "; std::cin >> sigma;
			std::cout << std::endl;
			std::cout << "Please choose an option: " << std::endl;
			std::cout << "\t1. Gradient magnitude (w/ local suppression)" << std::endl;
			std::cout << "\t2. Gradient Binary " << std::endl;
			std::cout << "<edges>: "; std::cin >> option;
			std::cout << std::endl;
			if (option == '1')
			{
				edge_gradients(bimg, sigma);
			} else if (option == '2') {
				std::cout << "warning: if input is less than one, it will be treated as a percentage" << std::endl;
				std::cout << "Please enter a threshold [0, 255] OR [0, 1) percentage: "; std::cin >> threshold;

				std::cout << std::endl;
				edge_gradients(bimg, sigma, threshold);
			} else {
				std::cout << "\nInvalid Option" << std::endl;
			}
			glutDestroyWindow(win);
			resetViewer();
			break;
		case 7:
			std::cout << "*/*\\* Welcome to Kmeans Segmentation! */*\\*" << std::endl;
			bimg = openImageDialog();
			std::cout << "Please enter k: "; std::cin >> k;
			std::cin.clear();
			
			std::cout << std::endl;
			if (k < 0 || k > 100)
			{
				k = 16;
				std::cout << "Invalid k value" << std::endl;
				std::cout << "k = " << k << std::endl;
			}
			std::cout << "..... Choices ....." << std::endl;
			std::cout << "\t1. Compute Kmeans now!" << std::endl;
			std::cout << "\t2. Enter a weight for intensity " << std::endl;
			std::cout << "\t3. Use a minimum distance between clusters instead of k " << std::endl; 
			std::cout << "Please choose an option: "; std::cin >> option;
			std::cout << std::endl;

			if (option == '1')
			{
				kmeans(bimg, k);
			} else if (option == '2') {
				std::cout << "Please enter a weight: "; std::cin >> sigma;
				std::cout << std::endl;
				kmeans(bimg, k, sigma);
			} else if (option == '3') {
				std::cout << "Please enter minimum distance: "; std::cin >> sigma;
				std::cout << std::endl;
				dmeans(bimg, sigma);
			} else {
				std::cout << "\nInvalid Option" << std::endl;
			}
			glutDestroyWindow(win);
			resetViewer();
			break;
		case 8:
			std::cout << "*/*\\* Welcome to Image Registration! */*\\*" << std::endl;

			// Input base image
			std::cout << "Please choose an image... ";
			bimg = openImageDialog();
			std::cout << "Thanks." << std::endl;
			glutDestroyWindow(win);
			resetViewer();
			//second = bimg;
			// For mouse function so that previously implemented algorithms still work
			usingRegistration = true;

			// Input secondary Image
			std::cout << "Please choose a secondary image...";
			second = openImageDialog();
			//viewer(second);
			//glutDestroyWindow(win2);
			//resetViewer();
			std::cout << "Thanks." << std::endl;
			
			std::cout << " Please input the number of points you would like to use (min 3): "; std::cin >> num_points;
			std::cout << std::endl;
			std::cout << " Please input the window size radius to use: "; std::cin >> k;
			std::cout << std::endl;
			if (num_points < 3)
				num_points = 3;
			// Get points
			// ** Three points are selected in mouse function
			std::cout << "Please select " << num_points << " points for registration (click the image)" << std::endl;



			// Call registration algorithm
			// ** Algorithm called after choosing three image points in mouse function

			break;
		case 99:
			exit(0);
	}
	glutPostRedisplay();
}


// Calls menu
void resetViewer() {

	initg(); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);

	if (usingRegistration)
	{
		win = glutCreateWindow("Reference Image");
		win2 = glutCreateWindow("Target image");
	} else
		win = glutCreateWindow("CEG-7550 Computer Vision");

	glutCreateMenu(menu);
	glutAddMenuEntry("Load Image", 1);
	glutAddMenuEntry("Save Image", 2);
	glutAddMenuEntry("Bilateral Filter Image", 3);
	glutAddMenuEntry("Gaussian Smoothing", 4);
	glutAddMenuEntry("Edge Detector", 5);
	glutAddMenuEntry("Edge Gradient (Extra Credit)", 6);
	glutAddMenuEntry("K-Means", 7);
	glutAddMenuEntry("Register two images", 8);
	glutAddMenuEntry("QUIT", 99);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

}

//Calls resetViewer, is called by main method
void viewer(const ByteImage& img) {
	bimg = img;
	resetViewer();
	glutMainLoop();
}
