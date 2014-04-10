#include <lshkit.h>
#include <lshkit/matrix.h>

typedef MultiProbeLshIndex<unsigned> Index;

typedef struct {
    int dim;
    Index *index;
    lshkit::FloatMatrix *data;    
    lshkit::FloatMatrix::Accessor *accessor;
    lshkit::metric::l1<float> *metric;
} Environment;


extern "C" {
    Environment *init(int dim, int N, float *data_block);
    void query(Environment *env, int queryIndex);
}
