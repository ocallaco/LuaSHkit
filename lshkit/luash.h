#include <lshkit.h>


typedef struct {
    int dim;
    void *index;
    FloatMatrix *data;    
} Environment;


extern "C" {
    Environment *init(int dim, int N, float *data_block);
    int query(Environment *env, float *queryData);
}
