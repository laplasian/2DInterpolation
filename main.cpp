#include "Interpolator2D.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    char *input_fn;
    char *output_fn;
    size_t Ndst;
    size_t i_type = 0;

    if (argc == 5) {
        input_fn = argv[1];
        output_fn = argv[2];
        Ndst = static_cast<size_t>(strtol(argv[3], nullptr, 10));
        i_type = static_cast<size_t>(strtol(argv[4], nullptr, 10));
        if (i_type >= COUNT) {
            return -1;
        };

    }
    else {
        printf("args error");
        return -1;
    }

    try {
        std::ifstream in_stream(input_fn, std::ifstream::in);
        Interpolator2D interpolator(Parser::get_data(in_stream));
        in_stream.close();
        //enum
        interpolator.Interpolate(Ndst, i_type);
        std::ofstream out_stream(output_fn, std::ostream::out);
        save_result(out_stream, interpolator.get_result());
        out_stream.close();

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }


    return 0;
}