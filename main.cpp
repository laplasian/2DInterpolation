#include "Interpolator2D.h"
#include <iostream>
#include <fstream>

// int main(int argc, char *argv[])
// {
//     char *input_fn;
//     char *output_fn;
//     size_t Ndst;
//     if (argc == 4) {
//         input_fn = argv[1];
//         output_fn = argv[2];
//         Ndst = static_cast<size_t>(strtol(argv[3], nullptr, 10));
//     }
//     else {
//         printf("args error");
//         return -1;
//     }
//
//     try {
//         Parser parser;
//         std::ifstream in_stream(input_fn, std::ifstream::in);
//         Interpolator2D interpolator2D(parser.get_data(in_stream));
//         in_stream.close();
//
//         interpolator2D.Bilinear(Ndst);
//         std::ofstream out_stream(output_fn, std::ofstream::out);
//         interpolator2D.save_result(out_stream);
//         out_stream.close();
//
//     } catch (std::exception &e) {
//         std::cout << e.what() << std::endl;
//         return -1;
//     }
//
//
//     return 0;
// }