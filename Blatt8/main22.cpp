#include <fstream>
#include <vector>
#include <iostream>
#include <mpi.h>
#include <chrono>
using namespace std::chrono;



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
std::vector<Type> merge_p_sorted_vecs(std::vector<Type> &vec) {
    int length_vec = vec.end() - vec.begin();
    bool is_sorted = false;
    while (!is_sorted) {
        is_sorted = true;
        for(int i = 0; i < length_vec-1; i++) {
            if (vec[i] > vec[i+1]) {
                Type tmp = vec[i];
                vec[i] = vec[i+1];
                vec[i+1] = tmp;
                is_sorted = false;
            }
        }
    }
    return vec;
}

int main_run(int argc, char **argv) {
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

        MPI_Scatter(&chunk_size, 1, MPI_INT, nullptr, nullptr, nullptr, 0, MPI_COMM_WORLD);

        std::vector<int> chunk_sizes(totalnodes, chunk_size);
        chunk_sizes[-1] = vec_length-chunk_size*(totalnodes-1);

        std::vector<int> chunk_start_idx(totalnodes, 0);
        for(int i = 1; i<totalnodes; i++) {
            chunk_start_idx[i] = chunk_size*i;
        }

        std::vector<uint64_t> sub_vec(chunk_size, 0);
        MPI_Scatterv(&vec[0], &chunk_sizes[0], &chunk_start_idx[0], MPI_UINT64_T, &sub_vec[0], chunk_size, MPI_UINT64_T, 0, MPI_COMM_WORLD);

        std::sort(sub_vec.begin(), sub_vec.end());
        std::vector<uint64_t> semisorted_vec(vec_length, 0);

        MPI_Gatherv(&sub_vec[0], chunk_size, MPI_UINT64_T, &semisorted_vec[0], &chunk_sizes[0], &chunk_start_idx[0], MPI_UINT64_T, 0, MPI_COMM_WORLD);

        std::vector<uint64_t> sorted_vec = merge_p_sorted_vecs(semisorted_vec);
        std::cout << "Sorted: " << std::is_sorted(sorted_vec.begin(), sorted_vec.end()) << std::endl;
    } else {
        int chunk_size;     // Retrieve the chunk size from the Status object
        MPI_Scatter(nullptr, 0, MPI_INT, &chunk_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        std::vector<uint64_t> vec(chunk_size, 0);

        MPI_Scatterv(nullptr, nullptr, nullptr, nullptr, &vec[0], chunk_size, MPI_UINT64_T, 0, MPI_COMM_WORLD);

        std::sort(vec.begin(), vec.end());

        MPI_Gatherv(&vec[0], chunk_size, MPI_UINT64_T, nullptr, nullptr, nullptr, nullptr, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}

int main(int argc, char **argv) {
    auto start = high_resolution_clock::now();
    for(int i = 0; i <10; i++) {
        int it_does_not_matter = main_run(argc, argv);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Mean execution time: " << duration.count()/10 << std::endl;
}