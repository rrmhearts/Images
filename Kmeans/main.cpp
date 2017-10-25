/* CEG-7550 Course Supplement */

/* Ryan McCoppin 's project 1
 * September 14, 2012
 * Computer Vision, CEG 7550
 * Due: October 11, 2012 @ 3:30pm

 */

/* Main Routine */
 
#include "define.h"

int main(int argc, char** argv) {

	// In dialogs.cpp, returns image
	ByteImage bimg = openImageDialog();	

	if(!bimg.image) return 0;//No image or invalid image selected

	// Give command arguments? What arguments
	glutInit(&argc, argv);

	// In viewer.cpp, displays or saves image
	viewer(bimg);
	
	return 0;
}



