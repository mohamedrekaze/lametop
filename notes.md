- how to check for file type?
    - use stat function
    it takes a `struct stat st` and the path for the file
    - then use functions to check for each type like
    `S_ISREG`, `S_ISDIR`, `S_ISLNK` by passing `st_mode` from `struct stat st`

