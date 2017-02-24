#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <list>
//#include <sys/types.h>
using namespace std;

char *me;

void usage(int ret)
{
    printf("\n"
           "usage: %s [options] -i input_dir -o output_dir\n"
           "  options:\n"
           "    -d        Run in DeltaFS mode\n"
           "    -n num    Number of particles to read (reading ID 1 to num)\n"
           "    -h        This usage info\n"
           "\n",
           me);

    exit(ret);
}

/*
 * TODO:
 * - include deltafs_api.h
 * - link lib properly in CMakeLists.txt
 */
int deltafs_read_particles(long long int num, char *indir, char *outdir)
{
#if 0
    int ret;
    deltafs_plfsdir_t *dir;
    char *file_data, fname[PATH_MAX];

    dir = deltafs_plfsdir_create_handle(O_RDONLY);

    if (!(ret = deltafs_plfsdir_open(dir, indir, NULL))) {
        perror("Error: cannot open input PLFS directory");
        deltafs_plfsdir_free_handle(dir);
        usage(1);
    }

    for (int i=1; i<=num, i++) {

        /* TODO: Determine and populate fname for EACH particle */

        if (!(file_data = deltafs_plfsdir_readall(dir, fname)) {
            perror("Error: failed to read particle data");
            deltafs_plfsdir_free_handle(dir);
            usage(1);
        }

        /* TODO: Write out particle trajectory data */

        free(file_data);
    }

    deltafs_plfsdir_free_handle(dir);
#endif /* 0 */
    return 0;
}

int process_epoch(char *ppath, char *outdir, int it, long long int num)
{
    /* TODO */
    return 0;
}

int read_particles(long long int num, char *indir, char *outdir)
{
    DIR *in;
    struct dirent *dp;
    char ppath[PATH_MAX];
    list<int> epochs;
    list<int>::iterator it;

    printf("Reading particles from %s.\n", indir);
    printf("Storing trajectories in %s.\n", outdir);

    /* Open particle directory and sort epoch directories */
    if (!snprintf(ppath, PATH_MAX, "%s/particle", indir)) {
        perror("Error: snprintf failed");
        usage(1);
    }

    if ((in = opendir(ppath)) == NULL) {
        perror("Error: cannot open input directory");
        usage(1);
    }

    while (dp = readdir(in)) {
        int epoch;
        char *end;

        if (dp->d_type != DT_DIR)
            continue;

        if (!strcmp(dp->d_name, ".") ||
            !strcmp(dp->d_name, "..") ||
            dp->d_name[0] != 'T')
            continue;

        epoch = strtoll(dp->d_name+2, &end, 10);
        if (*end) {
            perror("Error: strtoll failed");
            closedir(in);
            usage(1);
        }

        //printf("Found subdir %s, epoch %d.\n", dp->d_name, epoch);

        epochs.push_back(epoch);
    }

    closedir(in);
    epochs.sort();

    /* Iterate through epoch frames */
    for (it = epochs.begin(); it != epochs.end(); ++it) {
        printf("Processing epoch %d.\n", *it);
        if (process_epoch(ppath, outdir, *it, num)) {
            fprintf(stderr, "Error: epoch data processing failed\n");
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    int ret, c, d = 0;
    long long int num = 1;
    char indir[PATH_MAX], outdir[PATH_MAX];

    me = argv[0];
    indir[0] = outdir[0] = '\0';

    while ((c = getopt(argc, argv, "dhi:n:o:p:")) != -1) {
        switch(c) {
        case 'd': /* run in DeltaFS mode */
            d = 1;
            break;
        case 'h': /* print help */
            usage(0);
        case 'i': /* input directory (VPIC output) */
            if (!strncpy(indir, optarg, PATH_MAX)) {
                perror("Error: invalid input dir");
                usage(1);
            }
            break;
        case 'n': /* number of particles to fetch */
            char *end;
            num = strtoll(optarg, &end, 10);
            if (*end) {
                perror("Error: invalid num argument");
                usage(1);
            }
            break;
        case 'o': /* output directory (trajectory files) */
            if (!strncpy(outdir, optarg, PATH_MAX)) {
                perror("Error: invalid output dir");
                usage(1);
            }
            break;
        default:
            usage(1);
        }
    }

    if (!indir[0] || !outdir[0]) {
        fprintf(stderr, "Error: input and output directories are mandatory\n");
        usage(1);
    }

    /* Do particle things */
    if (d)
        ret = deltafs_read_particles(num, indir, outdir);
    else
        ret = read_particles(num, indir, outdir);

    return ret;
}