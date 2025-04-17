#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

std::string imageToBinaryChar(const cv::Mat& img) {
    std::string result = "";

    int rows = img.rows;
    int cols = img.cols;
    int cellHeight = rows / 8;
    int cellWidth = cols / 8;

    for (int row = 0; row < 8; ++row) {
        std::string binary = "";
        for (int col = 0; col < 8; ++col) {
            cv::Rect cell(col * cellWidth, row * cellHeight, cellWidth, cellHeight);
            cv::Mat roi = img(cell);
            double avg = cv::mean(roi)[0];
            binary += (avg > 128) ? '1' : '0';  // Threshold
        }
        // Convert binary to character
        char ch = static_cast<char>(std::stoi(binary, nullptr, 2));
        result += ch;
    }

    return result;
}

int main() {
    std::string folder = "extracted/";
    std::vector<std::string> imagePaths;

    for (const auto& entry : fs::directory_iterator(folder)) {
        imagePaths.push_back(entry.path().string());
    }

    // Sort image paths by name to keep order
    std::sort(imagePaths.begin(), imagePaths.end());

    std::string message = "";

    for (const std::string& path : imagePaths) {
        cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
        if (img.empty()) {
            std::cerr << "Failed to read image: " << path << std::endl;
            continue;
        }

        message += imageToBinaryChar(img);
    }

    std::cout << "Decoded message:\n" << message << std::endl;
    return 0;
}

