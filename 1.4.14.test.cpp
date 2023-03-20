#include "tasks.h"
#include <fstream>
#include "cstring"

uint64_t main1_4_14_test(int argc, char **argv) {
    int repeats = 5;

    for (int matrix_rank: {100, 500, 1000, 1500}) {
        std::fstream file("1_4_14_" + std::to_string(matrix_rank) + ".txt", std::ios::out);

        try {
            for (int i = 16; i > 0; i--) {
                char **custom_argv = new char *[2];
                custom_argv[0] = new char[10];
                custom_argv[1] = new char[5];
                itoa(i, custom_argv[1], 5);

                uint64_t sum = 0;
                for (int j = 0; j < repeats; ++j) {
                    uint64_t time = main1_4_14(2, custom_argv, matrix_rank);
                    sum += time;

                    std::cout << i << ":" << j << " - " << time << std::endl;
                    file << i << ":" << j << " - " << time << std::endl;
                }
                std::cout << "Average with " << i << " threads: " << (double) sum / repeats << "\n\n";
                file << "Average with " << i << " threads: " << (double) sum / repeats << "\n\n";
                file.flush();
                std::cout.flush();

                delete[] custom_argv[0];
                delete[] custom_argv[1];
                delete[] custom_argv;
            }
        } catch (std::exception &e) {
            file.close();
            std::cout << e.what();
            throw e;
        }
    }


    return 0;
}