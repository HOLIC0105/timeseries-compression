
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

#define CHUNK 16384

int main(int argc, char *argv[]) {
    const char *input_file = "data_data.txt";
    const char *output_file = "output.zip";
    char buffer[CHUNK];
    int ret, flush;
    unsigned have;
    z_stream strm;
    FILE *infile, *outfile;

    // 打开输入文件和输出文件
    infile = fopen(input_file, "rb");
    outfile = fopen(output_file, "wb");

    // 初始化 zlib 的数据结构
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        fprintf(stderr, "deflateInit failed\n");
        exit(EXIT_FAILURE);
    }

    // 压缩输入文件并输出到输出文件
    do {
        strm.avail_in = fread(buffer, 1, CHUNK, infile);
        if (ferror(infile)) {
            (void)deflateEnd(&strm);
            fclose(infile);
            fclose(outfile);
            exit(EXIT_FAILURE);
        }
        flush = feof(infile) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = (Bytef *)buffer;
        do {
            strm.avail_out = CHUNK;
            strm.next_out = (Bytef *)buffer;
            ret = deflate(&strm, flush);
            have = CHUNK - strm.avail_out;
            if (fwrite(buffer, 1, have, outfile) != have || ferror(outfile)) {
                (void)deflateEnd(&strm);
                fclose(infile);
                fclose(outfile);
                exit(EXIT_FAILURE);
            }
        } while (strm.avail_out == 0);
    } while (flush != Z_FINISH);
    (void)deflateEnd(&strm);

    // 关闭输入文件和输出文件
    fclose(infile);
    fclose(outfile);

    return 0;
}