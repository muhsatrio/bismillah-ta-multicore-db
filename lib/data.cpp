#include "data.h"

MPI_Datatype generateStructPoint() {
    MPI_Datatype data;
    int blocksCount = 2;
    int blocksLength[2] = {1, 1};

    MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};
    MPI_Aint offsets[2];
    offsets[0] = offsetof(point, x);
    offsets[1] = offsetof(point, y);

    MPI_Type_create_struct(blocksCount, blocksLength, offsets, types, &data);
    MPI_Type_commit(&data);
    return data;
}

MPI_Datatype generateStructLine() {
    MPI_Datatype data;
    int blocksCount = 2;
    int blocksLength[2] = {1, 1};

    MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};
    MPI_Aint offsets[2];
    offsets[0] = offsetof(line, m);
    offsets[1] = offsetof(line, c);

    MPI_Type_create_struct(blocksCount, blocksLength, offsets, types, &data);
    MPI_Type_commit(&data);
    return data;
}

MPI_Datatype generateStructVect(MPI_Datatype struct_point) {
    MPI_Datatype data;
    int blocksCount = 3;
    int blocksLength[3] = {1, 1, 1};

    MPI_Datatype types[3] = {struct_point, struct_point, MPI_UNSIGNED};
    MPI_Aint offsets[3];
    offsets[0] = offsetof(vect, start);
    offsets[1] = offsetof(vect, end);
    offsets[2] = offsetof(vect, from);

    MPI_Type_create_struct(blocksCount, blocksLength, offsets, types, &data);
    MPI_Type_commit(&data);
    return data;
}