## Question-1

There are 2 files `client.c` and `server.c`  . The type of connection used in this question is non-perisitant i.e every time client downloads some file the connection is closed and established again on the request. The following error handling have been taken care of:

- Socket creation Error
- Connection Error
- File opening Error
- If file is avaiable on server or not

**To run the code**

```bash
  cd Server
  gcc server.c
  ./a.out
  # Open new terminal
  cd Client
  gcc client.c
  ./a.out test.md 1.pdf 2.pdf 3.pdf 4.pdf 5.pdf
```

On running the code the client side will ask you to Enter the filename, the server will try to search your file in `Server/Data` folder. If the file is found in the path it wil be downloaded, otherwise it will display an error message, showing that such file doesn't exist on server.