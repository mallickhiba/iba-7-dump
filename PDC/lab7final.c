#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define S 10000000 // can only use datasize up to 10 million because of cocalc's resource allocation limits

int main(int argc, char **argv)
{
    int rank;
    int W; // number of workers available
    int *data = NULL;
    int local_data_size;
    long long local_dot_product = 0;
    long long global_dot_product = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &W);

    if (rank == 0)
    { // if supervisor
        data = (int *)malloc(sizeof(int) * S);
        for (int i = 0; i < S; i++)
        {
            data[i] = rand() % 100;
        }

        // distribute data
        int WL = S / W;
        for (int i = 1; i < W; i++)
        {
            MPI_Send(&data[(i - 1) * WL], WL, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        local_data_size = S - (W - 1) * WL;
        for (int i = (W - 1) * WL; i < S; i++)
        {
            local_dot_product += data[i];
        }
    }
    else
    { // if worker
        local_data_size = S / W;
        int *local_data = (int *)malloc(sizeof(int) * local_data_size);
        MPI_Recv(local_data, local_data_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < local_data_size; i++)
        {
            local_dot_product += local_data[i];
        }

        free(local_data);
    }

    MPI_Reduce(&local_dot_product, &global_dot_product, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Dot Product: %lld\n", global_dot_product);
    }

    MPI_Finalize();
    return 0;
}
