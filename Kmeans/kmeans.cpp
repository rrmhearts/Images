/* 
 * Ryan McCoppin 's project 3
 * October 25, 2012
 * Computer Vision, CEG 7550
 * Due: October 30, 2012 @ 3:30 PM

 */

#include "define.h"

void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}

// Kmeans
double kmeans(ByteImage& bimg, const int K)
{
	return kmeans(bimg, K, 1);
}

// Kmeans with weight option
double kmeans(ByteImage& bimg, const int K, double weight) {

	// Create k random points (centers)
	point * centers = new point[K];

	// Totals for updating
	point * totals = new point[K];

	// Initialize k points
	for (int k = 0; k < K; k++)
	{
		// Initiate centers
		centers[k].x = rand() % bimg.nr;
		centers[k].y = rand() % bimg.nc;
		centers[k].I = rand() % 256;

		// Initiate sumation for average
		totals[k].x = 0;
		totals[k].y = 0;
		totals[k].I = 0;
		totals[k].area = 0;
	}
	// Create label map
	byte * label = new byte[bimg.nr*bimg.nc];
	double shortestDistance, presentDistance;
	double error = bimg.nr*bimg.nr + bimg.nc*bimg.nc + 255*255, old_error;

	// Weights maybe? Set these to one for original kmeans.
	double wi = 1, wj = 1; //255.0*255/(double)bimg.nr/bimg.nr, wj = 255.0*255/(double)bimg.nc/bimg.nc;
	int closestK = 0;

	do {

		// Update previous error
		old_error = error;
		error = 0;
		// A. For each point in space, find distance to each center, assign to min center (map colors)
		for (int i = 0; i < bimg.nr; i++)
		{
			for (int j = 0; j < bimg.nc; j++)	
			{
				// Initialize shortestDistance to max possible.
				shortestDistance = sqrt(bimg.nr*bimg.nr + bimg.nc*bimg.nc + 255*255);
				// Check distance to each center
				for (int k = 0; k < K; k++)
				{
					// Calculate the distance
					presentDistance = sqrt((double)wi*(centers[k].x-i)*(centers[k].x-i) + 
										   (double)wj*(centers[k].y-j)*(centers[k].y-j) + 
								    (double)weight*(centers[k].I-bimg.image[i*bimg.nc+j])*(centers[k].I-bimg.image[i*bimg.nc+j]));

					// Update shortest distance
					if (presentDistance < shortestDistance)
					{
						shortestDistance = presentDistance;
						closestK = k;
						label[i*bimg.nc+j] = (255.0/K)*k;
					}
				}
				
				// Update error
				error += shortestDistance;

				// Update totals to compute new centroid
				totals[closestK].x += i;
				totals[closestK].y += j;
				totals[closestK].I += bimg.image[i*bimg.nc+j];
				totals[closestK].area += 1;
			}
		}
		error = error / bimg.nr / bimg.nc;
		// Update centers
		for (int k = 0; k < K; k++)
		{
			// Compute new centers
			if (totals[k].area > 0)
			{
				centers[k].x = totals[k].x / totals[k].area;
				centers[k].y = totals[k].y / totals[k].area;
				centers[k].I = totals[k].I / totals[k].area;
			}

			// Reset totals
			totals[k].x = 0;
			totals[k].y = 0;
			totals[k].I = 0;
			totals[k].area = 0;
		}

	// B. Repeat A until centers do not move
	} while(error != old_error);

// Extra credit code to find and "average" distance between the nodes, which must be less than user input
	std::cout << "K=" << K << std::endl;

	// Farthest distance
	shortestDistance = sqrt(bimg.nr*bimg.nr + bimg.nc*bimg.nc + 255*255);
	for (int i = 0; i < K; i++)
	{
		for (int j = i+1; j < K-1; j++)
		{
			if (j != i)
			{
				// Present distance between nodes
				presentDistance = sqrt((double)wi*(centers[i].x-centers[j].x)*(centers[i].x-centers[j].x) + 
									   (double)wj*(centers[i].y-centers[j].y)*(centers[i].y-centers[j].y) + 
									   weight*(double)(centers[i].I-centers[j].I)*(centers[i].I-centers[j].I));
				// Find smallest distance
				if (presentDistance < shortestDistance)
				{
				
					// My average idea.
					shortestDistance = 0.66*shortestDistance + .33*presentDistance;
				}
			}
		}
	} // end extra credit code 1, see function below for the use of this.
	// Make label image
	bimg.image = label;
	delete [] totals;
	delete [] centers;

	std::cout << shortestDistance << std::endl;
	return shortestDistance;
}

// Extra credit, use various k values to find solution with a specified "average" between cluster centers.
void dmeans(ByteImage& bimg, const int mindist)
{
	dmeans(bimg, mindist, 1);
}

// Extra credit, use various k values to find solution with a specified "average" between cluster centers.
// Optional weights
void dmeans(ByteImage& bimg, const int mindist, double weight)
{
	ByteImage temp = bimg;
	int k = 3;
	double dist;

	// While average distance between clusters is greater than some min value, loop
	while (dist=kmeans(temp, k, weight) > mindist)
	{
		temp = bimg;
		std::cout << dist << std::endl;
		k++;
	}

	// Assign final image to bimg
	bimg = temp;
	std::cout << "K has be chosen to be " << k << "." << std::endl;
}