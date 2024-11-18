#include <fstream>
#include <vector>
#include <iostream>
#include <mpi.h>



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

template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;
 
    std::vector<T> vec(first, last);
    return vec;
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
        const int chunk_size = vec_length/totalnodes + (vec_length % totalnodes > 0);

        for (int i = 1; i < totalnodes-1; i++) {
            MPI_Send(&vec[chunk_size*i], chunk_size, MPI_UINT64_T, i, 0, MPI_COMM_WORLD);
        }
        MPI_Send(&vec[chunk_size*(totalnodes-1)], (vec_length-chunk_size*(totalnodes-1)), MPI_UINT64_T, (totalnodes-1), 0, MPI_COMM_WORLD);

        auto sub_vector = slice(vec, 0, chunk_size);
        uint64_t sum_sums = sum_func(sub_vector);
        uint64_t sum = 0;
        for (int i = 1; i < totalnodes; i++) {
            MPI_Recv(&sum, 1, MPI_UINT64_T, i, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum_sums = sum_sums + sum;
        }

        float total_mean = sum_sums/vec_length;
        std::cout << "Mean: " << total_mean << std::endl;
    } else {
        MPI_Status status;
        MPI_Probe(
            0,              // Rank of the process sending the chunk; in our case, rank 0
            0,              // Tag of the message
            MPI_COMM_WORLD, // Communicator
            &status         // Pointer to the status object
        );
        int chunk_size;     // Retrieve the chunk size from the Status object
        MPI_Get_count(&status, MPI_UINT64_T, &chunk_size);
        std::vector<uint64_t> vec(chunk_size, 0);
        MPI_Recv(&vec[0], chunk_size, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        uint64_t sum = sum_func(vec);
        MPI_Send(&sum, 1, MPI_UINT64_T, 0, 42, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}