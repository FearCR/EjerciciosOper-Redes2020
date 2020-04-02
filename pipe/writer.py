#!/usr/bin/python3.8
# writer.py
import os
from message import create_msg

if __name__ == "__main__":
    IPC_FIFO_NAME = "myfifo"

    #fifo = os.open(IPC_FIFO_NAME, os.O_WRONLY)
    try:
        while True:
            fifo = os.open(IPC_FIFO_NAME, os.O_WRONLY)
            name = input("Enter a name: ")
            content = f"Hello {name}!"#.encode("utf8")
            msg = content.encode();
            os.write(fifo, msg)
            os.close(fifo)
    except KeyboardInterrupt:
        print("\nGoodbye!")
    finally:
        os.close(fifo)
