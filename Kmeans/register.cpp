/* 
 * Ryan McCoppin 's project 3
 * October 25, 2012
 * Computer Vision, CEG 7550
 * Due: October 30, 2012 @ 3:30 PM

 */

#include "define.h"

void registration(ByteImage& bimg1, ByteImage& bimg2, std::vector<point> pts, int sigma) {
	std::cout << "***** Registration begin ********" << std::endl;
	// The points (pts) are from bimg1.

	ByteImage resampled = bimg1;
	int i, j, k, ke, l, le, window_size = sigma;
	int temp_i, temp_j;
	double sum=0, average=0, area = 0, r, win_norm, t_norm;
	double a=0,b=0,c=1,d=2,e=3,f=4;
	double na=1,nb=1,nc=0,nd=2,ne=3,nf=1;
	double nna=1,nnb=1,nnc=0,nnd=2,nne=3,nnf=1;
	std::vector<point> corr;

	// Compute template mean
	for (int i = 0; i < pts.size(); i++)
	{
		sum = 0;
		pts[i].r = -1;
		// Set row bound for window size
		if (pts[i].x-window_size < 0) pts[i].radx = pts[i].x;
		else if (pts[i].x+window_size > bimg1.nc) pts[i].radx = min(pts[i].radx, bimg1.nc-pts[i].x);
		else pts[i].radx = window_size;

		// Set column bound for window size
		if (pts[i].y-window_size < 0) pts[i].rady=pts[i].y;
		else if (pts[i].y+window_size > bimg1.nr) pts[i].rady = min(pts[i].rady, bimg1.nr-pts[i].y);
		else pts[i].rady = window_size;

		// Pixels over
		pts[i].area = 0;//4*pts[i].radx*pts[i].rady;

		// For a window in Bimg2, find average
		for (k=-pts[i].rady; k < pts[i].rady; k++) // rows
		{
			for (l=-pts[i].radx; l < pts[i].radx; l++) //cols
			{
				sum += bimg1.image[(pts[i].y+k)*bimg1.nc+(pts[i].x+l)];
				pts[i].area ++;
			}
		}
		pts[i].avg = sum / pts[i].area;
		display_point(pts[i]);
	} // end computing template data
	std::cout << "..... Please wait for program to finish...." << std::endl;
	// To BECOME window best correlated
	// Used to save the correlated points
	point p;
	for (int i = 0; i < pts.size(); i++)
	{
		corr.push_back(p);
	}
	// For each point in Bimg2
	for (i = 0; i < bimg2.nr; i++) //rows
	{
		// 1. Compute means for window
		// 2. Compute correlation for each chosen pixel
		// 3. Keep best in Bimg2

		for (j = 0; j < bimg2.nc; j++) //cols
		{
// 1
			sum = 0; average = 0; area = 0;
			// Set row bound for window size
			if (i-window_size < 0) k = -i;
			else k = -window_size;

			if (i+window_size > bimg2.nr) ke = i+window_size-bimg2.nr;
			else ke = window_size;

			// Set column bound for window size
			if (j-window_size < 0) l = -j;
			else l = -window_size;

			if (j+window_size > bimg2.nc) le = j+window_size-bimg2.nc;
			else le = window_size;

			// Pixels over
			area = 0;//(ke - k)*(le-l);

			// For a window in Bimg2, find average
			for (k; k < ke; k++) // rows
			{
				// Set column bound for window size
				if (j-window_size < 0) l = -j;                     //changed i to j Wed
				else l = -window_size;
				for (l; l < le; l++) //cols
				{
					sum += bimg2.image[(i+k)*bimg2.nc+(j+l)];
					area ++;
				}
			}

			// Average intensity for area around pixel (i,j) in WINDOW
			average = sum / area;
// 2
			// For each template point find correlation to template, keep best
			for (int p = 0; p < pts.size(); p++)
			{
				sum = 0; area = 0;
				win_norm = 0; t_norm = 0;

				// Set row bound for window size
				if (i-window_size < 0) k = -i; // min
				else k = -window_size;

				if (i+window_size > bimg2.nr) ke = i+window_size-bimg2.nr; // max
				else ke = window_size;

				// Set column bound for window size
				if (j-window_size < 0) l = -j;  // min                             //changed i to j
				else l = -window_size;

				if (j+window_size > bimg2.nc) le = j+window_size-bimg2.nc; //max                //changed i to j
				else le = window_size;
// Compute r

				// For a window in Bimg2
				k = max(k, -pts[p].rady);
				ke = min(ke, pts[p].rady);
				for (k; k < ke; k++) // rows
				{
					if (j-window_size < 0) l = -j;  // min
					else l = -window_size;
					l = max(l, -pts[p].radx);
					le = min(le, pts[p].radx);
					for (l; l < le; l++) //cols
					{
						// Compute correlation coefficient parts
						temp_i = pts.at(p).y; //rows
						temp_j = pts.at(p).x; //cols
						// Compute numerator. Template term                                            Window term
						sum += ((double)bimg1.image[(temp_i+k)*bimg1.nc+(temp_j+l)] - pts.at(p).avg) * ((double)bimg2.image[(i+k)*bimg2.nc+(j+l)] - average);

						// Compute denominator. Template term                                         Window sum computed earlier
						t_norm += ((double)bimg1.image[(temp_i+k)*bimg1.nc+(temp_j+l)] - pts.at(p).avg)*((double)bimg1.image[(temp_i+k)*bimg1.nc+(temp_j+l)] - pts.at(p).avg);
						win_norm += ((double)bimg2.image[(i+k)*bimg2.nc+(j+l)] - average)*((double)bimg2.image[(i+k)*bimg2.nc+(j+l)] - average);
					}
				}

				// Compute r. Sqrt denom.
				r = sum/pow(t_norm*win_norm,.5);

				// If r is more correlated than previous point
				if ( r > pts.at(p).r)
				{
					// Update correlated point
					pts[p].r = r;
					corr[p].r = r;
					corr[p].x = j;
					corr[p].y = i;
					corr[p].I = bimg2.image[i*bimg2.nc+j];
					
				} // end best corr
			} // end template points
		} // end Bimg2 cols
	} // end Bimg2 rows

	for (int i = 0; i < pts.size(); i++)
	{
		std::cout << "(" << pts[i].x << ", " << pts[i].y << ") => (" << corr[i].x << ", " << corr[i].y << "), r: " << pts[i].r << std::endl;

	}
	// Find best match point in Bimg2

	// Calculate parameters
	//double error, best_error = 9999;
	//for (int i = 0; i < pts.size()-2; i++)
	//{
		//sum = (pts[1].x-pts[2].x)*(corr[0].x-corr[1].x) + (pts[1].y-pts[0].y)*(pts[1].x-pts[2].x) + (pts[1].y-pts[2].y)*(pts[0].x-pts[1].x);
		//b = (corr[1].x-corr[2].x)*(pts[0].x-pts[1].x) / sum;
		//a = ((corr[0].x-corr[1].x) - b*(pts[0].y-pts[1].y)) / (pts[0].x-pts[1].x);
		//c = corr[2].x - a*pts[2].x - b*pts[2].y;

		//sum = (pts[1].x-pts[2].x)*(corr[0].y-corr[1].y) + (pts[1].y-pts[0].y)*(pts[1].x-pts[2].x) + (pts[1].y-pts[2].y)*(pts[0].x-pts[1].x);
		//e = (corr[1].y-corr[2].y)*(pts[0].x-pts[1].x) / sum;
		//d = ((corr[0].y-corr[1].y) - b*(pts[0].y-pts[1].y)) / (pts[0].x-pts[1].x);
		//f = corr[2].y - a*pts[2].x - b*pts[2].y;
	/*
		error = (corr[i].x - a*pts[i].x - b*pts[i].y -c)*(corr[i].x - a*pts[i].x - b*pts[i].y -c) +
				(corr[i].y - d*pts[i].x - e*pts[i].y -f)*(corr[i].y - d*pts[i].x - e*pts[i].y -f);*/
	//	if (error < best_error)
	//	{
	//		best_error = error;
	//		na=a, nb=b, nc=c,nd=d,ne=e,nf=f;
	//	}
	//}
	//a=na, b=nb, c=nc,d=nd,e=ne,f=nf;


	// New attept at calculating parameters
	/*double error, best_error = 9999;
	for (int i = 0; i < pts.size()-2; i++)
	{
		c = ((double)corr[i+2].x*pts[i+1].x*(pts[i+i].x*pts[i+1].y - pts[i+i].y*pts[i+2].x) + 
			corr[i+1].x*pts[i+1].x*(pts[i].y*pts[i+2].x + pts[i].x*pts[i+2].y) + 
			corr[i].x*pts[i+1].x*(pts[i+1].y*pts[i+2].x - pts[i+1].x*pts[i+2].y)) / (double)
			(pts[i].x*pts[i+1].x*(pts[i+1].y-pts[i+2].y) + 
			pts[i+1].x*pts[i+2].x*(pts[i].y-pts[i+2].y) + 
			pts[i+1].x*pts[i+1].x*(pts[i+2].y-pts[i].y));

		b = ((double)corr[i+1].x*pts[i].x - 
			corr[i].x*pts[i+1].x - (pts[i].x-pts[i+1].x)*c) / (double)
			(pts[i].x*pts[i+1].y - pts[i].y*pts[i+1].x);
		a = (corr[i].x - b*pts[i].y - c) / pts[i].x;

		f = ((double)corr[i+2].y*pts[i+1].x*(pts[i].x*pts[i+1].y - pts[i].y*pts[i+2].x) + 
			corr[i+1].y*pts[i+1].x*(pts[i].y*pts[i+2].x + pts[i].x*pts[i+2].y) + 
			corr[i].y*pts[i+1].x*(pts[i+1].y*pts[i+2].x - pts[i+1].x*pts[i+2].y)) / (double)
			(pts[i].x*pts[i+1].x*(pts[i+1].y-pts[i+2].y) + 
			pts[i+1].x*pts[i+2].x*(pts[i].y-pts[i+2].y) + 
			pts[i+1].x*pts[i+1].x*(pts[i+2].y-pts[i].y));

		e = ((double) corr[i+1].y*pts[i].x - 
			corr[i].y*pts[i+1].x - (pts[i].x-pts[i+1].x)*f) / (double)
			(pts[i].x*pts[i+1].y - pts[i].y*pts[i+1].x);
		d = (corr[i].y - e*pts[i].y - f) / pts[i].x;

		error = (corr[i].x - a*pts[i].x - b*pts[i].y -c)*(corr[i].x - a*pts[i].x - b*pts[i].y -c) +
				(corr[i].y - d*pts[i].x - e*pts[i].y -f)*(corr[i].y - d*pts[i].x - e*pts[i].y -f);
		if (error < best_error)
		{
			best_error = error;
			na=a, nb=b, nc=c,nd=d,ne=e,nf=f;
		}
	}
	a=na, b=nb, c=nc,d=nd,e=ne,f=nf;*/
		c = ((double)corr[2].x*pts[1].x*(pts[0].x*pts[1].y - pts[0].y*pts[2].x) + 
			corr[1].x*pts[1].x*(pts[0].y*pts[2].x + pts[0].x*pts[2].y) + 
			corr[0].x*pts[1].x*(pts[1].y*pts[2].x - pts[1].x*pts[2].y)) / (double)
			(pts[0].x*pts[1].x*(pts[1].y-pts[2].y) + 
			pts[1].x*pts[2].x*(pts[0].y-pts[2].y) + 
			pts[1].x*pts[1].x*(pts[2].y-pts[0].y));

		b = ((double)corr[1].x*pts[0].x - 
			corr[0].x*pts[1].x - (pts[0].x-pts[1].x)*c) / (double)
			(pts[0].x*pts[1].y - pts[0].y*pts[1].x);
		a = (corr[0].x - b*pts[0].y - c) / pts[0].x;

		f = ((double)corr[2].y*pts[1].x*(pts[0].x*pts[1].y - pts[0].y*pts[2].x) + 
			corr[1].y*pts[1].x*(pts[0].y*pts[2].x + pts[0].x*pts[2].y) + 
			corr[0].y*pts[1].x*(pts[1].y*pts[2].x - pts[1].x*pts[2].y)) / (double)
			(pts[0].x*pts[1].x*(pts[1].y-pts[2].y) + 
			pts[1].x*pts[2].x*(pts[0].y-pts[2].y) + 
			pts[1].x*pts[1].x*(pts[2].y-pts[0].y));

		e = ((double) corr[1].y*pts[0].x - 
			corr[0].y*pts[1].x - (pts[0].x-pts[1].x)*f) / (double)
			(pts[0].x*pts[1].y - pts[0].y*pts[1].x);
		d = (corr[0].y - e*pts[0].y - f) / pts[0].x;
	//std::cout << std::endl;

	std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
	std::cout << "d: " << d << ", e: " << e << ", f: " << f << std::endl;

	// Map second image to new image.

	int X, Y;
	int count = 0;
	for (i = 0; i < resampled.nr; i++)
	{

		for (j = 0; j < resampled.nc; j++)
		{
			X = floor(a*j+b*i+c);
			Y = floor(d*j+e*i+f);
			if (X > 0 && X < bimg2.nc && Y > 0 && Y < bimg2.nr)
			{
				//std::cout << "X: " << X << ", Y: " << Y << std::endl;
				resampled.image[i*resampled.nc+j] = (char)floor(((double)resampled.image[i*resampled.nc+j] + (double)bimg2.image[Y*bimg2.nc+X])/2);
				count ++;
			}
		}
	}
	std::cout << "overlap point count: " << count << std::endl;

	bimg1 = resampled;

	std::cout << "******* End Registration method ********" << std::endl;
}
