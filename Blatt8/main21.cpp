#include <fstream>
#include <vector>
#include <iostream>
#include <mpi.h>

// mpic++ -std=c++11 -o MPI main21.cpp
// mpirun MPI

template <class Type>
size_t length_of_file(const std::string &path) {
    std::fstream stream(path, std::ios::binary | std::ios::in | std::ios::ate);
    size_t length = stream.tellg() / sizeof(Type);
    stream.close();
    return length;
}

// Reads a binary file that stores a sequence of values with type 'Type' and returns
// them as a std::vector<Type>
template <class Type>
std::vector<Type> read_vec(const std::string &path) {
    const size_t size = length_of_file<Type>(path);
    std::vector<Type> to_read(size);

    std::ifstream read_stream(path, std::ios::binary);
    read_stream.read(reinterpret_cast<char *>(&to_read[0]), size * sizeof(Type));
    read_stream.close();

    return to_read;
}

template <class Type>
Type sum_func(std::vector<Type> &vec) {
    int length = vec.end() - vec.begin();
    Type sum = 0;
    for (Type i: vec) {
        sum = sum + i;
    }
    return sum;
}

int main(int argc, char **argv) {
    int mynode, totalnodes;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    std::cout << "Proc. " << mynode << " of " << totalnodes << " reports for service" << std::endl;

    if(mynode == 0) {
        // Read the vector
        const auto vec = read_vec<uint64_t>("arrays/array-1.bin");
        const int vec_length = vec.end() - vec.begin();

        // chunksize with last chunk has same or larger size
        int chunk_size = vec_length/totalnodes + (vec_length % totalnodes > 0);

        MPI_Scatter(&chunk_size, 1, MPI_INT, &chunk_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        std::vector<int> chunk_sizes(totalnodes, chunk_size);
        chunk_sizes[-1] = vec_length-chunk_size*(totalnodes-1);

        std::vector<int> chunk_start_idx(totalnodes, 0);
        for(int i = 1; i<totalnodes; i++) {
            chunk_start_idx[i] = chunk_size*i;
        }

        std::vector<uint64_t> sub_vec(chunk_size, 0);
        MPI_Scatterv(&vec[0], &chunk_sizes[0], &chunk_start_idx[0], MPI_UINT64_T, &sub_vec[0], chunk_size, MPI_UINT64_T, 0, MPI_COMM_WORLD);

        uint64_t sum = sum_func(sub_vec);
        uint64_t sum_sums = 0;
        MPI_Reduce(&sum, &sum_sums, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        float total_mean = sum_sums/vec_length;
        std::cout << "Mean: " << total_mean << std::endl;
    } else {
        int chunk_size;     // Retrieve the chunk size from the Status object
        MPI_Scatter(nullptr, 0, MPI_INT, &chunk_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        std::vector<uint64_t> vec(chunk_size, 0);

        std::cout << "chunk size: " << chunk_size << std::endl;

        MPI_Scatterv(nullptr, nullptr, nullptr, nullptr, &vec[0], chunk_size, MPI_UINT64_T, 0, MPI_COMM_WORLD);

        uint64_t sum = sum_func(vec);

        std::cout << "Sum: " <<  sum << std::endl;

        MPI_Reduce(&sum, nullptr, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}