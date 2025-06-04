#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> apply_filter_cpp(const std::vector<int>& data, int width, int height, const std::string& filter_name) {
    std::vector<int> result = data;

    if (filter_name == "invert") {
        for (size_t i = 0; i + 3 < result.size(); i += 4) {
            result[i] = 255 - result[i];
            result[i + 1] = 255 - result[i + 1];
            result[i + 2] = 255 - result[i + 2];
        }
    }

    if (filter_name == "blur") {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int r = 0, g = 0, b = 0, a = 0, count = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = std::min(std::max(x + dx, 0), width - 1);
                        int ny = std::min(std::max(y + dy, 0), height - 1);
                        int idx = (ny * width + nx) * 4;
                        r += data[idx];
                        g += data[idx + 1];
                        b += data[idx + 2];
                        a += data[idx + 3];
                        count++;
                    }
                }
                int idx = (y * width + x) * 4;
                result[idx] = r / count;
                result[idx + 1] = g / count;
                result[idx + 2] = b / count;
                result[idx + 3] = a / count;
            }
        }
    }

    if (filter_name == "blue") {
        for (size_t i = 0; i + 3 < result.size(); i += 4) {
            int r = result[i];
            int g = result[i + 1];
            int b = result[i + 2];

            if (r > 240 && g > 240 && b > 240) {
                result[i]     = 0;     // R
                result[i + 1] = 0;     // G
                result[i + 2] = 255;   // B
            } else if (r < 30 && g < 30 && b < 30) {
                result[i]     = 255;   // R
                result[i + 1] = 255;   // G
                result[i + 2] = 0;     // B
            }
        }
    }

    if (filter_name == "grey") {
        for (size_t i = 0; i + 3 < result.size(); i += 4) {
            int avg = (result[i] + result[i + 1] + result[i + 2]) / 3;
            result[i] = avg;
            result[i + 1] = avg;
            result[i + 2] = avg;
        }
    }

    return result;
}

PYBIND11_MODULE(filter, m) {
    m.def("apply_filter_cpp", &apply_filter_cpp, "Apply C++ filter to image data");
}
