#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <map>
#include <string>

using namespace std;
using namespace cv;

typedef map<string, string>::const_iterator MapIterator;
DIR *dir;
struct dirent *ent;
const char* image_window = "Source Image";
Mat hero,heroes,result;

//Make map of directory files
int makeMap(map<string,string> &heroesMap){
	if ((dir = opendir ("./heroes")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if (strstr(ent->d_name, ".png") != NULL){
				heroesMap[ent->d_name] = ent->d_name;
				//printf ("%s\n", ent->d_name);
			}
		}
		closedir (dir);
	} else {
	  /* could not open directory */
	  perror ("");
	  return 0;
	}
	return 1;
}

void printMap(map<string,string> &heroesMap){	
	for (MapIterator iter = heroesMap.begin(); iter != heroesMap.end(); iter++)
	{
	    cout << "Key: " << iter->first << endl << "Values:" << iter->second <<endl;
	}
}


void matchLoc(Point &point){
	int result_cols =  heroes.cols - hero.cols + 1;
	int result_rows = heroes.rows - hero.rows + 1;

	result.create( result_rows, result_cols, CV_32FC1 );


	matchTemplate( heroes, hero, result, TM_SQDIFF);

	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	
	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	point = minLoc;
}



void drawMap(map<string,string> &heroesMap){
	heroes = imread( "screen_heroes.png", IMREAD_COLOR );
	for (MapIterator iter = heroesMap.begin(); iter != heroesMap.end(); iter++)
	{
		hero = imread("./heroes/"+iter->first,IMREAD_COLOR);
		if(hero.empty())
		{
			cout << "Can't read one of the images" << endl;
		}
		Point match;
		matchLoc(match);

		rectangle( heroes, match, Point( match.x + hero.cols , match.y + hero.rows ), Scalar::all(255), 2, 8, 0 );
		circle(heroes, Point(match.x + hero.cols / 2, match.y + hero.rows / 2 ), 5, Scalar::all(255));

  		

		cout << "Name:" << iter->first << endl << "Coords: x - " << match.x << " y - " << match.y << endl;

	    //cout << "Key: " << iter->first << endl << "Values:" << iter->second <<endl << "Path:" << "./heroes/"+iter->first << endl;
	}

	imshow( image_window, heroes );
}

int main(){
	map<string,string> heroesMap;
	int result = makeMap(heroesMap);
	//printMap(heroesMap);

	namedWindow( image_window, WINDOW_AUTOSIZE );

	drawMap(heroesMap);

	while (waitKey(0)){};
	return result; 
}

