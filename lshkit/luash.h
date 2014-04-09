#include <lshkit.h>
#include <lshkit/matrix.h>

typedef struct {
    int dim;
    void *index;
    lshkit::FloatMatrix *data;    
    lshkit::FloatMatrix::Accessor *accessor;
    lshkit::metric::l1<float> *metric;
} Environment;


extern "C" {
    Environment *init(int dim, int N, float *data_block);
    int query(Environment *env, float *queryData);
}
