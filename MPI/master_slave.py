from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()


if rank == 0:
    data=["Maikel", "Vagner", "Gabriel"]
    comm.send(data, dest=1)

else:
    data = comm.recv(source=0)
    print(data)
