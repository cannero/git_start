#include "cache.h"

void print_usage()
{
    printf("usage:\n--to_hex <sha1> convert sha1 to hex\n");
}

int read_index_file(){
    int fd, i;
    void *map;
    struct cache_header *hdr;
    struct stat st;
    unsigned long size, offset;

    const char *sha1_file_directory = getenv(DB_ENVIRONMENT);
    if (!sha1_file_directory){
        sha1_file_directory = DEFAULT_DB_ENVIRONMENT;
    }
#ifndef BGIT_WINDOWS
    fd = open(".dircache/index", O_RDONLY );
    map = (void *)-1;
#else
    fd = open(".dircache/index", O_RDONLY | O_BINARY );
        map = (void *) NULL;
#endif
    if (!fstat(fd, &st)) {
        map = NULL;
        size = st.st_size;

#ifndef BGIT_WINDOWS
        map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
#else
        void *fhandle 
            = CreateFileMapping((HANDLE) _get_osfhandle(fd), 
                                 NULL, PAGE_READONLY, 0, 0, NULL );
        map = MapViewOfFile(fhandle, FILE_MAP_READ, 0, 0, size);
        CloseHandle(fhandle);
#endif
    }
    close(fd);
    hdr = map;
    printf("header entries: %i, sha1 %s\nversion %i\n",
           hdr->entries,
           sha1_to_hex(hdr->sha1),
           hdr->version);

    offset = sizeof(*hdr);
    for (i = 0; i < hdr->entries; i++) {
        struct cache_entry *ce = map + offset;
        offset = offset + ce_size(ce);
        printf("cache entry: %s, sha1 %s\n",
               ce->name,
               sha1_to_hex(ce->sha1));

    }

    return 0;
}

int only_one_flag(char *arg)
{
    if (!strcmp(arg, "--read_index")){
        return read_index_file();
    }
    print_usage();
    return -1;
}

int main(int argc, char *argv[])
{
    if (argc == 2) {
        return only_one_flag(argv[1]);
    }
    else if (argc == 3){
        char *arg = argv[1];
        if (strcmp(arg, "--to_hex")){
            print_usage();
            return -1;
        }
        printf("hex is %s" ,sha1_to_hex((unsigned char*)argv[2]));
        return 0;
    }
    else{
        print_usage();
        return -1;
    }
}
