#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

int main(int argc, char* argv[]) {
    std::ifstream fs;
    if (argc > 1) {
        fs.open(argv[1]);
    }
    else {
        fs.open("input.txt");
    }

    int sum = 0;
    int power_sum = 0;

    fs.seekg(4, std::ios_base::cur);
    while (!fs.eof()) {
        int game_id, cube_count, min_red, min_green, min_blue;
        std::string cube_color;
        bool possible = true;

        min_red = 0;
        min_green = 0;
        min_blue = 0;

        fs >> game_id;
        if (fs.fail()) {
            break;
        }
        fs.seekg(2, std::ios_base::cur);
        do {
            fs >> cube_count;
            fs >> cube_color;
            if (cube_color[0] == 'r') {
                min_red = std::max(cube_count, min_red);
                if (cube_count > MAX_RED) {
                    possible = false;
                }
            }
            else if (cube_color[0] == 'g') {
                min_green = std::max(cube_count, min_green);
                if (cube_count > MAX_GREEN) {
                    possible = false;
                }
            }
            else if (cube_color[0] == 'b') {
                min_blue = std::max(cube_count, min_blue);
                if (cube_count > MAX_BLUE) {
                    possible = false;
                }
            }
            else {
                std::cout << "Unknown cube color " << cube_color << std::endl;
                return -1;
            }
        } while (cube_color.back() == ';' || cube_color.back() == ',');
        if (possible) {
            sum += game_id;
        }
        power_sum += min_red*min_green*min_blue;
        fs.seekg(5, std::ios_base::cur);
    }

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Power sum: " << power_sum << std::endl;

    return 0;
}