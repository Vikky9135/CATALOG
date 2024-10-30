#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <cmath>

using json = nlohmann::json;

// Function to convert a number in any base to decimal
long long convertToDecimal(const std::string& value, int base) {
    long long decimalValue = 0;
    int length = value.size();

    // Convert value to decimal based on the base
    for (int i = 0; i < length; ++i) {
        char digit = value[length - 1 - i];
        int digitValue;

        // Convert character to integer value based on the character
        if (isdigit(digit)) {
            digitValue = digit - '0'; // '0' to '9'
        } else if (isalpha(digit)) {
            digitValue = tolower(digit) - 'a' + 10; // 'a' to 'z'
        } else {
            digitValue = 0; // For safety; could handle as error if needed
        }

        // Make sure the digit is within the valid range for the base
        if (digitValue >= base) {
            // If the digit exceeds the base, we can stop further processing
            // and return 0, which will result in an invalid calculation
            return 0; 
        }

        decimalValue += digitValue * static_cast<long long>(pow(base, i));
    }

    return decimalValue;
}

// Lagrange interpolation to find the constant term
long long lagrangeInterpolation(const std::vector<std::pair<int, long long>>& points, int k) {
    long long constantTerm = 0;
    int n = points.size();

    for (int i = 0; i < k; ++i) {
        int xi = points[i].first;
        long long yi = points[i].second;

        double Li = 1.0;
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                int xj = points[j].first;
                Li *= static_cast<double>(0 - xj) / (xi - xj);
            }
        }
        constantTerm += yi * Li;
    }

    return constantTerm;
}

void processTestCase(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return;
    }

    json j;
    inputFile >> j;

    // Extract n and k
    int n = j["keys"]["n"];
    int k = j["keys"]["k"];
    std::vector<std::pair<int, long long>> points;

    // Process each numbered key to extract points
    for (const auto& element : j.items()) {
        if (element.key() != "keys") {  // Skip the "keys" element
            int x = std::stoi(element.key());
            int base = std::stoi(element.value()["base"].get<std::string>());
            std::string value = element.value()["value"].get<std::string>();
            long long y = convertToDecimal(value, base);
            points.emplace_back(x, y);
        }
    }

    // Calculate the constant term using the first k points
    long long constantTerm = lagrangeInterpolation(points, k);

    // Output the result
    std::cout << "Test case from " << filename << ":\n";
    std::cout << "{\"constant_term\": " << constantTerm << "}\n";
}

int main() {
    // Process both test cases
    processTestCase("testcase1.json");
    processTestCase("testcase2.json");
    return 0;
}
