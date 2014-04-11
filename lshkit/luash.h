#include <lshkit.h>
#include <lshkit/matrix.h>

typedef lshkit::MultiProbeLshIndex<unsigned> Index;

typedef struct {
    int dim;
    unsigned K;
    float R;
    Index *index;
    lshkit::FloatMatrix *data;    
    lshkit::FloatMatrix::Accessor *accessor;
    lshkit::metric::l1<float> *metric;
} Environment;


extern "C" {
    Environment *init(int dim, int N, float *data_block, unsigned K, float R, bool doTuning, const char *indexFile, int L, int T, int M, float W);
    void query(Environment *env, float *queryData, int *response);
}
