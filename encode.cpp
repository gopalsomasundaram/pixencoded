#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <filesystem>  // C++17

namespace fs = std::filesystem;

std::string charToBinary(char c) {
    std::string binary;
    for (int i = 7; i >= 0; --i) {
        binary += ((c >> i) & 1) ? '1' : '0';
    }
    return binary;
}

void binaryToImage(const std::string& chunk, const std::string& filename) {
    const int gridSize = 8;
    const int cellSize = 50;
    cv::Mat image(gridSize * cellSize, gridSize * cellSize, CV_8UC1, cv::Scalar(0));

    for (size_t row = 0; row < chunk.size(); ++row) {
        std::string binary = charToBinary(chunk[row]);
        for (size_t col = 0; col < binary.size(); ++col) {
            if (binary[col] == '1') {
                cv::rectangle(
                    image,
                    cv::Point(col * cellSize, row * cellSize),
                    cv::Point((col + 1) * cellSize - 1, (row + 1) * cellSize - 1),
                    cv::Scalar(255),
                    cv::FILLED
                );
            }
        }
    }

    cv::imwrite(filename, image);
    std::cout << "Saved: " << filename << std::endl;
}

void processString(const std::string& input) {
    const std::string start = "[START]";
    const std::string stop = "[STOP]";
    const size_t chunkSize = 8;
    const std::string outputDir = "encoded_imgs";

    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    int frameCounter = 1;
    char filename[100];

    // Start frame
    std::string paddedStart = start;
    paddedStart.resize(chunkSize, ' ');
    std::snprintf(filename, sizeof(filename), "%s/frame_%02d.png", outputDir.c_str(), frameCounter++);
    binaryToImage(paddedStart, filename);

    // Content frames
    for (size_t i = 0; i < input.size(); i += chunkSize) {
        std::string chunk = input.substr(i, chunkSize);
        chunk.resize(chunkSize, ' ');
        std::snprintf(filename, sizeof(filename), "%s/frame_%02d.png", outputDir.c_str(), frameCounter++);
        binaryToImage(chunk, filename);
    }

    // Stop frame
    std::string paddedStop = stop;
    paddedStop.resize(chunkSize, ' ');
    std::snprintf(filename, sizeof(filename), "%s/frame_%02d.png", outputDir.c_str(), frameCounter++);
    binaryToImage(paddedStop, filename);
}


int main(int argc, char* argv[]) {
    std::vector<std::string> all_inp;
    if (argc > 1){
    all_inp.assign(argv+1,argv+argc);
    }
    std::string input = "";
    for(const auto& x : all_inp){
        input.append(x);
        input.append(" ");
    }
    std::cout<<input;
    processString(input);

    std::cout << "All frames saved in 'encoded_imgs/' directory." << std::endl;
    return 0;
}

