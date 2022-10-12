#pragma once
#include<iostream>
#include<ctime>
#include<windows.h>
#include <gdiplus.h>
#include <objidl.h>
#include<string.h>
#include <sstream>
#include<thread>
#include <filesystem>

#include <opencv2/opencv.hpp>


#include"brain.h"

using namespace cv;
using namespace Gdiplus;
using namespace std;
using std::filesystem::directory_iterator;
namespace fs = std::filesystem;


#pragma comment (lib,"Gdiplus.lib")




//pic 60x75 pixs


#define saveFile true
#define imageCount 1
#define trainPack 1



class nnOperation
{
public:

	bool runTrain = true,
		 loadFile= false;

	float score = 0;
	float packDeviation = 0;
	float bestScore = 0.35f;
	float input[1800];
	float output[1] = { 0 };
	int gen = 0;
	int t = 0;

	bool training =false,
		showResults =false;

	bool setup=true;
	cv::Size imageSize = {60,75};
	brain nn;

	float scores[imageCount];




	int pixSum(int r, int c, Mat pic, int col) {
		int sum = 0;
		for (int i = r - 2; i < r + 2; i++) {
			for (int j = c - 2; j < c + 2; j++) {
				sum += pic.at<Vec3b>(i, j)[col];
			}
		}
		return (sum);
	}
	
	void showImage(HDC hdc, string str) {

		

		//HBRUSH brush;
		//HPEN pen;
		//pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
		//brush = CreateSolidBrush(RGB(255, 255, 255));
		//SelectObject(hdc, brush);
		//Rectangle(hdc, 0, 0, 800, 600);
		/*
		wstring wideusername;
		for (int i = 0; i < str.length(); ++i)
			wideusername += wchar_t(str[i]);

		const wchar_t* wstr = wideusername.c_str();
		*/
		std::wstring result;
		std::string::const_iterator end = str.end();
		for (std::string::const_iterator it = str.begin(); it != end; ++it) {
			result += to_unicode(*it);
		}
		//std::string str1(result.begin(), result.end());

		Graphics graphics(hdc);
		Image image(result.c_str());
		graphics.DrawImage(&image, 10, 10, 500, 500);

	
		
	}
	
	const wchar_t* strTowchar(std::string str) {
		wstring wideusername;
		for (int i = 0; i < str.length(); ++i)
			wideusername += wchar_t(str[i]);

		const wchar_t* wstr = wideusername.c_str();
		return (wstr);
	}

	void inputImage(float inp[],string str, HDC hdc) {

		/*std::stringstream ss;
		ss << dir << n << ".jpg";
		std::string str = ss.str();
		*/
		showImage(hdc, str);
		//printf("\n flag\n", n);

		Mat original = imread(str, CV_LOAD_IMAGE_COLOR);
		resize(original, original, imageSize);
		//imshow("image", original);
		//waitKey();


		int sum = 0;
		int i = 0;
		for (int r = 2; r < original.rows; r += 5) {
			for (int c = 2; c < original.cols; c += 5) {
				sum = pixSum(r, c, original, 0);
				inp[i] = sum / 9.0f;
				//	printf("i=%5d\t", i);
				i++;

				sum = pixSum(r, c, original, 1);
				inp[i] = sum / 9.0f;
				//	printf("i=%5d\t", i);
				i++;

				sum = pixSum(r, c, original, 2);
				inp[i] = sum / 9.0f;
				//	printf("i=%5d\t", i);
				i++;

				//	printf("r=%4d\tc=%4d\n", r, c);
			}
		}
		//printf("\n\nr=%5d\tc=%5d\n", original.rows, original.cols);
	}

	void loadScores(float scr[], string dir) {

		ifstream inFile;
		string scorePath =  dir.append("_scores.txt");
		inFile.open(scorePath);
		if (!inFile) {
			cout << "\nUnable to open 'scores' file\n";
			exit(1); // terminate with error
		}

		for (int i = 0; i < imageCount; i++) {
			inFile >> scr[i];
			scr[i] = scr[i] / 10.0f;
		}
		inFile.close();
	}

	void train(HDC hdc,HWND hLbl, string dir) {

		loadScores(scores, dir);
		
		float wantedScores[1];
		int n;
		score = 0;
		
		
		for (int i = 0; i < trainPack; i++) {
			if (training) {

				//n = i;
				n = rand() % imageCount;
				std::stringstream ss;
				ss << dir << n << ".jpg";
				std::string str = ss.str();
				ss.str("");
				
				wantedScores[0] = scores[n];
				inputImage(input, str, hdc);
				nn.thinK(input, output);
				nn.backProp(wantedScores,trainPack);


				if (abs(output[0] * 10 - wantedScores[0] * 10) > score)
					score = abs(output[0] * 10 - wantedScores[0] * 10);

				
				
				ss << "Result " << output[0] * 10 << "\nWanted " << wantedScores[0] * 10 << "\nDeviation " << abs(output[0] * 10 - wantedScores[0] * 10)<<"\nPack Deviation "<<packDeviation ;
				std::string printStr = ss.str();
				
				SetWindowTextA(hLbl, printStr.c_str());
				/*t++;
				if (t > 1000) {

					if (score > bestScore * 23) {
						//system("cls");
						//nn.initialize();
						gen++;
						//printf("\nscore %2f\t generation %d\n", score, gen);
						t = 0;
						
						


					}
					else if (t > 100000)
						if (score > bestScore * 5) {
						//system("cls");
						//nn.initialize();
						gen++;
						//printf("\nscore %2f\t generation %d\n", score, gen);
						t = 0;

						
						if (saveFile) {
							//nn.saveToFile();

							//	printf("\n\n___SAVED TO FILE___\n\n");
						}


					}
					else {
						if (t > 500000) {
							if (score > bestScore) {
								//	system("cls");
								gen++;
								//	printf("\nscore %2f\t generation %d\n", score, gen);
							//	nn.initialize();
								if (saveFile) {
							//		nn.saveToFile();
									
									//	printf("\n\n___SAVED TO FILE___\n\n");
								}
							}
							else if (score < bestScore - 0.05f) {
								if (saveFile) {
							//		nn.saveToFile();
									runTrain = false;
									//	printf("\n\n___SAVED TO FILE___\n\n");
								}

							}
							t = 0;
						}
					}
				}*/
			}
			
		}
		packDeviation = score;
		

	}

	void results(HDC hdc, HWND hLbl, string dir) {
		loadScores(scores, dir);
		nn.loadFromFile();
		float wantedScores[1];
		
		for (int i = 0; i < imageCount; i++) {
				if (showResults) {
				wantedScores[0] = scores[i];
				std::stringstream ss;
				ss << dir <<i << ".jpg";
				std::string str = ss.str();
				ss.str("");
				inputImage(input, str, hdc);
				nn.thinK(input, output);

				ss << "Result " << output[0] * 10 << "\nWanted " << wantedScores[0] * 10 << "\nDeviation " << abs(output[0] * 10 - wantedScores[0] * 10);
				std::string printStr = ss.str();
				SetWindowTextA(hLbl, printStr.c_str());
				Sleep(500);
			}
		}
	}

	string dirPath() {
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find(L'x64');
		//wstring pos1 = std::wstring(buffer).append(L"\\florist\\trainSamples\\");
		wstring wstr = std::wstring(buffer).substr(0, pos);
		string str(wstr.begin(), wstr.end());;

		return (str);
	}

	string openfilename(const char* filter = "All Files (*.*)\0*.*\0", HWND owner = NULL) {
		OPENFILENAMEA ofn;
		char fileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = owner;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "";

		string fileNameStr;

		if (GetOpenFileNameA(&ofn))
			fileNameStr = fileName;
		
		return fileNameStr;
	}

	void evaluateImage(HDC hdc,HWND hLbl) {
	
		string str = openfilename();

		nn.loadFromFile();

		inputImage(input, str, hdc);
		nn.thinK(input, output);
		std::stringstream ss;
		ss << "Result " << output[0] * 10;
		std::string printStr = ss.str();
		ss.str("");

		SetWindowTextA(hLbl, printStr.c_str());


		
	}

	void makeTrainingSamples(HWND hLbl) {
	
		int i = 0;
		string path = dirPath() + "\\floristApp\\trainSamples";
		directory_iterator end_itr;
		for (directory_iterator itr(path); itr != end_itr; ++itr)
		{
			fs::path path1 {itr->path() };
			std::string path_string{ path1.string() };
			if ((path_string != dirPath() + "\\floristApp\\trainSamples\\_scores.txt")&(path_string != dirPath() + "\\floristApp\\trainSamples\\buffer")) {
				rename(path_string.c_str(), (dirPath() + "\\floristApp\\trainSamples\\buffer\\" + to_string(i) + ".jpg").c_str());
				i++;

			}
			
		}

		i = 0;
		path = dirPath() + "\\floristApp\\trainSamples\\buffer";
		for (directory_iterator itr(path); itr != end_itr; ++itr)
		{
			fs::path path2{ itr->path() };
			std::string path_string2{ path2.string() };
			rename(path_string2.c_str(), (dirPath() + "\\floristApp\\trainSamples\\" + to_string(i) + ".jpg").c_str());
			i++;

		
		}




	}

	wchar_t to_unicode(char ch) {
		if (ch >= 0) return ch; //Если это первые 128 символов
		if (ch >= 'А' && ch <= 'я') return 1040 + ch - 'А';
		if (ch == 'ё') return 1105;
		if (ch == 'Ё') return 1025;
		return ch;
	}



	void trainOp(HDC hdc, HWND hlbl) {




		srand(time(0));
		string dir = dirPath().append("\\floristApp\\trainSamples\\");
		
		



		while (runTrain)
		{
			if (setup) {
				setUp();
				setup = !setup;
			}

			if (training) {
				train(hdc,  hlbl, dir);
			}
			else if (showResults) {
				results(hdc, hlbl, dir);
			}

		}

	}

	void setUp() {
		if (loadFile) {
			nn.loadFromFile();
		}
		else {
			nn.initialize();
		}
	}









};

