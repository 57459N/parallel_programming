#include <iostream>

int main1_4_14(int argc, char** argv){
#ifdef _OPENMP
    std::cout << "OPEN_MP SUPPORT";
#endif
    return 0;
}
