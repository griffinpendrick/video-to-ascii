#include <iostream>
#include <raylib.h>
#include <opencv2/opencv.hpp>

#define MAX_FILEPATH        4096

char ASCII[10] = {' ', '.', ':', '-', '=', '+', '*', '%', '@', '#'};

std::string frameToASCII(const cv::Mat& frame, int outputWidth, int outputHeight){

    cv::Mat resized;

    cv::resize(frame, resized, cv::Size(outputWidth / 5, outputHeight / 8));

    std::string ascii;
    for(int y = 0; y < resized.rows; ++y){
        for(int x = 0; x < resized.cols; ++x){
            cv::Vec3b pixel = resized.at<cv::Vec3b>(y, x);
            
            int r = pixel[2];
            int g = pixel[1];
            int b = pixel[0];

            float luminance = 0.2126 * r + 0.7152 * g + 0.0722 * b;

            int index = (int)(luminance * 9 / 255);

            ascii += ASCII[index];
        }
        ascii += '\n';
    }

    return ascii;
}

void drawFrame(std::string asciiFrame, Font font){
    float y = 0.0f;

    for (size_t start = 0, end = 0; end != std::string::npos; start = end + 1){
        end = asciiFrame.find('\n', start);
        std::string line = asciiFrame.substr(start, end - start);

        DrawTextEx(font, line.c_str(), {0.0f, y}, font.baseSize, 1.0f, WHITE);

        y += font.baseSize;
    }
}

int main(){

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Video to ASCII");
    SetTargetFPS(60);

    Font font = LoadFontEx("resources/Monospace.ttf", 8, 0, 250);

    char filePath[MAX_FILEPATH];

    cv::VideoCapture capture;
    cv::Mat frame;


    while(!WindowShouldClose()){

        int windowWidth = GetScreenWidth();
        int windowHeight = GetScreenHeight();

        
        if(IsFileDropped()){

            if(capture.isOpened())
                capture.release();

            FilePathList droppedFile = LoadDroppedFiles();

            TextCopy(filePath, droppedFile.paths[0]);
            capture.open(filePath);

            UnloadDroppedFiles(droppedFile);
        }

        capture >> frame;

        if(frame.empty())
            capture.release();

        BeginDrawing();

            if(capture.isOpened()){
                ClearBackground(BLACK);

                std::string asciiFrame = frameToASCII(frame, windowWidth, windowHeight);
                drawFrame(asciiFrame, font);
            }
            else{
                ClearBackground(GetColor(0x181818));
                DrawText("Drag & Drop", (windowWidth / 2.0f) - (MeasureText("Drag & Drop", 50) / 2.0f), (windowHeight / 2.0f) - 25, 50, WHITE);

            }

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
