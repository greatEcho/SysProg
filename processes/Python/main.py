import sys
import os
import subprocess

# const
ARGC = 3

def read_input_file(filename) -> tuple:
    try:
        with open(filename, "rb") as f:
            buffer = f.read()
            file_size = len(buffer)
        return buffer, file_size
    except IOError as e:
       print("Error: ", e, file=sys.stderr)
       sys.exit(1)

def create_file_chunks(buffer, file_size, num_child):
    shift = 0
    for i in range(num_child):
        if (i+1 == num_child):
            chunk_size = file_size - (num_child - 1) * (file_size // num_child)
        else:
            chunk_size = file_size // num_child
        with open(f"chunk{i}", "wb") as f:
            f.write(buffer[shift:shift+chunk_size])
        shift += chunk_size


def main():
    # check if args are passed
    if len(sys.argv) != ARGC:
        print(f"Usage: {sys.argv[0]} <input_file> <num_child>", file=sys.stderr)
        sys.exit(1)
    # check if the third arg is valid
    try:
        num_child = int(sys.argv[2])
    except ValueError as e:
       print("Error: ", e, file=sys.stderr)
       sys.exit(1)

    if num_child <= 0:
       print("Error: The number of child processes should be more than 0.", file=sys.stderr)
       sys.exit(1)

    # read input file
    buffer, file_size = read_input_file(sys.argv[1]) 
    if file_size <= 1:
        print("Error: File size is too small", file=sys.stderr)
        sys.exit(1)
    if file_size < num_child:
        num_child = file_size // 2
        print(f"Warning: The number of child processes is too high. "
              f"The altered value will be {num_child}.", file=sys.stderr)

    # create file chunks
    create_file_chunks(buffer, file_size, num_child)

    # create child processes
    children = list();
    for i in range(num_child):
        try:
            cwd = os.getcwd()
            subproc = os.path.join(cwd, "subproc.py")
            child = subprocess.Popen(["python", subproc, f"chunk{i}"], shell=True)
        except OSError as e:
            print(f"Error {e.errno}: Can't launch child process", file=sys.stderr)
            sys.exit(1)
        children.append(child)
        print(f"New process created with process ID {child.pid}")

    # Wait until child processes exit
    for child in children:
        return_code = child.wait()
        if return_code:
            print(f"Child process finished with error code {return_code}.", file=sys.stderr)
            sys.exit(1)


    print(f"Parent process with ID {os.getpid()} continuing..")
    # read results from child processes
    result = 0
    for i in range(num_child):
        try:
            with open(f"chunk{i}_result", "rb") as chunk_file:
                result += int(chunk_file.readline())
        except (IOError, ValueError) as e:
            print("Error: ", e, file=sys.stderr)
    
    print("-> Answer is %d <-\n" % result);

    sys.exit(0)


if __name__ == "__main__":
    main()