#include "Interpolator2D.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    char *input_fn;
    char *output_fn;
    size_t Ndst;
    if (argc == 4) {
        input_fn = argv[1];
        output_fn = argv[2];
        Ndst = static_cast<size_t>(strtol(argv[3], nullptr, 10));
    }
    else {
        printf("args error");
        return -1;
    }

    try {
        Interpolator2D interpolator2D(input_fn);

        interpolator2D.Bilinear(Ndst);
        interpolator2D.save_result(output_fn);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }


    return 0;
}