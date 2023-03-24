import sys

def read_chunk_file(filename: str) -> int:
    # read file and count desired ASCII symbols
    counter = 0
    try:
        with open(filename, 'r') as chunk_file:
            while True:
                c = chunk_file.read(1) # read byte by byte
                if not c:
                    break
                if ((ord(c) >= 0 and ord(c) <= 0x1F) or (ord(c) == 0x7F)):
                    counter += 1
        return counter
    except IOError as e:
       print("Error: ", e, file=sys.stderr)
       sys.exit(1)
    
def write_result(result: int, filename: str) -> None:
    result_filename = f"{filename}_result"
    try:
        with open(result_filename, 'w') as output_file:
            output_file.write(str(result))
    except IOError as e:
       print("Error: ", e, file=sys.stderr)
       sys.exit(1)


def main():
    # check if args are passed
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <input_file>", file=sys.stderr)
        sys.exit(1)

    filename = sys.argv[1]
    # read file and count desired ASCII symbols
    result = read_chunk_file(filename)
    # write result in the output file
    write_result(result, filename)

    sys.exit(0)

if __name__ == "__main__":
    main()